#include <opencv2/opencv.hpp>

cv::Point intersect_determinant(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4) {
    // Handle if the lower determnant is zero
    float det = ((p1.x - p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x - p4.x));
    if (det == 0) {
        return cv::Point(0, 0);
    }
    // See https://en.wikipedia.org/wiki/Line–line_intersection for formula
    float px = ((p1.x*p2.y - p1.y*p2.x)*(p3.x - p4.x) - (p1.x - p2.x)*(p3.x*p4.y - p3.y*p4.x))/det;
    float py = ((p1.x*p2.y - p1.y*p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x*p4.y - p3.y*p4.x))/det;
    return cv::Point(px, py);
}


int main() {
    cv::Point p1(0, 10);
    cv::Point p2(100, 30);
    cv::Point p3(3, 0);
    cv::Point p4(7, 30);

    cv::Point intersection = intersect_determinant(p1, p2, p3, p4);

    std::cout << "x: " << intersection.x << "y: " << intersection.y << std::endl;

    return 0;
}

