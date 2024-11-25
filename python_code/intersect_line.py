def intersection_determinant(line1, line2):
    x1, y1, x2, y2 = line1
    x3, y3, x4, y4 = line2
    
    det = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    if det == 0:
        return None
    
    # From https://en.wikipedia.org/wiki/Line–line_intersection
    px = (((x1*y2) - (y1*x2))*(x3-x4) - (x1-x2)*((x3*y4) - (y3*x4))) / det
    py = (((x1*y2) - (y1*x2))*(y3-y4) - (y1-y2)*((x3*y4) - (y3*x4))) / det
    return px, py
    
line1 = (90, -758, 1167, -709)  # Line defined by points (x1,y1) and (x2, y2)
line2 = (643, 60, 643, -1000)   # Ball's path defined by points (x3, y3) and (x4, y4)

intersection2 = intersection_determinant(line1, line2)

print(f"Intersection: {intersection2}")
