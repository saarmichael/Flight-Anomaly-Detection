
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);

// true if point is in or on the circle, false otherwise
bool isPointInCircle(const Point& point, const Circle& circle);

// find a circle such that all three points are on its radius
Circle threePCircle(const Point& p1, const Point& p2, const Point& p3);

// find a circle such that the two points are on its radius
Circle twoPCircle(const Point& p1, const Point& p2);

// true if all points are in the circle, flase otherwise
bool arePointsInCircle(const vector<Point>& points, const Circle& circle);

// creates a circle from one, two or three points
Circle basicCircle(vector<Point>& points);

// recursive function based on Welzl's algorithm
Circle findMinHelper(vector<Point>& insidePoints, vector<Point> bounderyPoints, size_t n);

float distance(Point p1, Point p2);

#endif /* MINCIRCLE_H_ */
