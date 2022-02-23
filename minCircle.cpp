#include "minCircle.h"


// true if point is in or on the circle, false otherwise
bool isPointInCircle(const Point& point, const Circle& circle) {
    return distance(point, circle.center) <= circle.radius;  
}

// true if all points are in the circle, flase otherwise
bool arePointsInCircle(const vector<Point>& points, const Circle& circle) {
    for(const Point& p : points) {
        if (!isPointInCircle(p, circle))
        {
            return false;
        }
    }
    return true;
}


// find a circle such that the two points are on its radius
Circle twoPCircle(const Point& p1, const Point& p2) {
    float xCenter = (p1.x + p2.x) / 2.0;
    float yCenter = (p1.y + p2.y) / 2.0;
    Point p1Copy(p1.x, p1.y);
    Point p2Copy(p2.x, p2.y);
    float radius = distance(p1Copy, p2Copy) / 2.0;
    return {{xCenter, yCenter}, radius};
}

// find a circle such that all three points are on its radius
Circle threePCircle(const Point& p1, const Point& p2, const Point& p3) {
    float Xdiff1 = p2.x - p1.x;
    float Ydiff1 = p2.y - p1.y;
    float Xdiff2 = p3.x - p1.x;
    float Ydiff2 = p3.y - p1.y;
    float a = (Xdiff1 * Xdiff1) + (Ydiff1 * Ydiff1);
    float b = (Xdiff2 * Xdiff2) + (Ydiff2 * Ydiff2); 
    float c = (Xdiff1 * Ydiff2) - (Ydiff1 * Xdiff2);
    float x = ((Ydiff2 * a) - (Ydiff1 * b)) / (c * 2);
    float y = ((Xdiff1 * b) - (Xdiff2 * a)) / (c * 2);
    x += p1.x;
    y += p1.y;
    Point center(x, y);
    return {center, distance(p1, center)};

}


// creates a circle from one, two or three points
Circle basicCircle(vector<Point>& points) {
    int size = points.size();
    if (size > 3)
    {
        throw "can not create a circle from more than 3 points";
    }
    if (size == 0)
    {
        // return 'zero-circle'
        return {{0,0}, 0};
    }
    if (size == 1)
    {
        // return 'point circle'
        return{points[0], 0};
    }
    if (size == 2)
    {
        return twoPCircle(points[0], points[1]);
    }
    
    // if size == 3, there are four options:
    // 1) p[0] & p[1] forms a circles that contains p[2]
    // 2) p[0] & p[2] forms a circles that contains p[1] 
    // 3) p[1] & p[2] forms a circles that contains p[0]
    // 4) all three points forms the minimmal circle
    for(int i = 0; i < 3; i++)
    {
        Circle c = twoPCircle(points[i], points[(i + 1) % 3]);
        if (arePointsInCircle(points, c))
        {
            return c;
        }
    }
    return threePCircle(points[0], points[1], points[2]);    
}

// recursive function based on Welzl's algorithm
Circle findMinHelper(vector<Point>& insidePoints, vector<Point> bounderyPoints, size_t n) {

    if (0 == n || 3 == bounderyPoints.size())
    {
        return basicCircle(bounderyPoints);
    }
    // choose the last Point as its an arbitrary Point
    Point p = {insidePoints[n - 1].x, insidePoints[n - 1].y};
    // recursive call
    Circle c = findMinHelper(insidePoints, bounderyPoints, n - 1);

    if (isPointInCircle(p, c))
    {
        return c;
    }
    // else, the point should be on the boundery of the circle
    bounderyPoints.push_back(p);
    return findMinHelper(insidePoints, bounderyPoints, n - 1);
    
}

Circle findMinCircle(Point** points, size_t size) {
    vector<Point> pointsCopy;
    for (size_t i = 0; i < size; i++) {
        float x = points[i]->x;
        float y = points[i]->y;
        Point p(x, y);
        pointsCopy.push_back(p);
    }
    vector<Point> bounds;
    return findMinHelper(pointsCopy, {}, size);
}

float distance(Point p1, Point p2) {
    float Xdiff = p1.x - p2.x;
    float Ydiff = p1.y - p2.y;
    return sqrtf((Xdiff * Xdiff) + (Ydiff * Ydiff));
}