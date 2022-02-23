// Michael Saar
#include <iostream>
#include <math.h>

// returns the average of X values
float avg(const float *x, int size);

// returns the variance of X and Y
float var(const float *x, int size);

// returns the covariance of X and Y
float cov(const float *x, const float *y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(const float *x, const float *y, int size);

class Line
{
public:
    float a, b;
    Line() : a(0), b(0) {}
    Line(float a, float b) : a(a), b(b) {}
    float f(float x)
    {
        return a * x + b;
    }
};

class Point
{
public:
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size);
Line linear_reg(const float * xEs, const float *yEs , int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

// // return the distance between two points
// float distance(Point p1, Point p2);