
#include <opencv2/opencv.hpp>
#include <lccv.hpp>

// avg FPS at HD is 125 without computation of center, with center computing, 111.111, and at FHD is 48.478
#define CAMERA_HEIGHT 720    // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280    // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 100 // If fps higher than what the thread can handle, it will just run lower fps.

#define SECONDS_ACTIVE 360


/**
 * @brief Function to calculate the center of a contour using moments
 * @param contour - The contour to calculate the center of
 * @return The center of the contour as cv::Point
 */
cv::Point calculateCenter(const std::vector<cv::Point> &contour)
{
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0)
    {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    return cv::Point(0, 0); // Return a dummy value if contour area is zero
}

cv::Point simplePredict(cv::Point old_ball_pos, cv::Point new_ball_pos) {
    return cv::Point(new_ball_pos.x + 5*(new_ball_pos.x - old_ball_pos.x), new_ball_pos.y + 5*(new_ball_pos.y - old_ball_pos.y));
}

cv::Point intersect_determinant(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4) {
    // Handle if the lower determnant is zero
    float det = ((p1.x - p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x - p4.x));
    if (det == 0) { // if determinant is zero, these lines are parallel
        return cv::Point(-1, -1); // The lines are parallele
    }
    // See https://en.wikipedia.org/wiki/Line–line_intersection for formula
    float px = ((p1.x*p2.y - p1.y*p2.x)*(p3.x - p4.x) - (p1.x - p2.x)*(p3.x*p4.y - p3.y*p4.x))/det;
    float py = ((p1.x*p2.y - p1.y*p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x*p4.y - p3.y*p4.x))/det;
    return cv::Point(px, py); // Not necessary to return the y value, but it is there for debugging purposes
}


int main() {
    bool running = true;

    std::cout << "Press ESC to stop. (Does not work if no window is displayed)" << std::endl;

    cv::Mat image, mask, HSV;
    lccv::PiCamera cam;

    cv::Scalar hsv_lower(0, 150, 50); // was (0, 120, 120)
    cv::Scalar hsv_upper(15, 255, 255); // was (10, 255, 255)

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;

    cv::namedWindow("Video", cv::WINDOW_FULLSCREEN);
    cv::namedWindow("Mask", cv::WINDOW_FULLSCREEN);

    cam.startVideo();

    cv::Point new_center(0, 0);
    cv::Point old_center(0, 0);

    cv::Point ball_intersect(0, 0);
    cv::Point predictball_pos(0, 0);

    // cv::Point temp_pos(0, 0);

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cerr << "Timeout error" << std::endl;
        }
        else {
            cv::cvtColor(image, HSV, cv::COLOR_BGR2HSV);

            cv::inRange(HSV, hsv_lower, hsv_upper, mask);

            cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);
            cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

            // Find contours in the mask
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


            if (!contours.empty()) {
                // Find the largest contour
                std::vector<cv::Point> largestContour = *std::max_element(contours.begin(), contours.end(), 
                [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });
                new_center = calculateCenter(largestContour);
            }

            // display the calculated center
            // cv::circle(image, new_center, 5, cv::Scalar(0, 255, 0), -1);

            // cv::putText(image, "Center", new_center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
            // cv::imshow("Video", image);


            if (new_center.x == 0 && new_center.y == 0) {
                continue;
            }

            if (abs(new_center.x - old_center.x) < 2 && abs(new_center.y - old_center.y) < 2) {
                old_center = new_center;
                continue;
            }

            predictball_pos = simplePredict(old_center, new_center);


            if (old_center.y - new_center.y  < 0) { // If the ball is moving towards the goal
                ball_intersect = intersect_determinant(cv::Point(85, 760), cv::Point(1167, 709), old_center, predictball_pos);
            } else {
                ball_intersect = cv::Point(0, 0); // Reset?
            }

            // Only add intersect point if it is going into the goal
            if (ball_intersect.x > 455 && ball_intersect.x < 830) {
                // deque_ball_pos.push(ball_intersect);
                std::cout << "Intersect: " << "x: " << ball_intersect.x << " y: " << ball_intersect.y << std::endl;
                cv::line(image, ball_intersect, new_center, cv::Scalar(0, 0, 255), 20);
            } else {
                std::cout << "No ball intersection" << std::endl;
            }

            cv::imshow("Video", image);
            cv::imshow("Mask", mask);

            // show image with the tracked object
            // quit on q button
            if (cv::waitKey(1) == 'q') {
                break;
            }

            old_center = new_center;
        }
        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE + 1}) {
            running = false;
        }
    }

    cam.stopVideo();
    // cv::destroyWindow("Video");
    // cv::destroyWindow("Mask");
    cv::destroyAllWindows();
    // deque_ball_pos.push(cv::Point(0, 0)); // Send a dummy value to stop the fussball thread
    std::cout << "End of opencv" << std::endl;
}