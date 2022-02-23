// Bareket Damari
// Michael Saar

#include <iostream>
#include "anomaly_detection_util.h"
#include <math.h>


// returns the average of X values
float avg(const float *x, int size) {

    float sum = 0;

    for (int i = 0; i < size; i++) {
        sum += x[i];
    }

    return sum / (float)size;
}

// returns the variance of X and Y
float var(const float *x, int size) {
    
    // sum of x's values each to the power of 2
    float result = 0;

    for (int i = 0; i < size; i++) {
        result += x[i] * x[i];
    }

    result = result / (float)size;
    float average = avg (x, size);
    result = result - (average * average);
    return result;
}

// returns the covariance of X and Y
float cov(const float *x, const float *y, int size) {
    
    float xAvg = avg(x, size);
    float yAvg = avg(y, size);

    float result = 0;

    for (int i = 0; i < size; i++) {
        result += (x[i] - xAvg) * (y[i] - yAvg);
    }

    return result / (float)size;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(const float *x, const float *y, int size){
    return cov(x, y, size) / (sqrtf(var(x, size)) * sqrtf(var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size)
{
    float *xEs = new float[size];
    float *yEs = new float[size];

    // seperating the points to their components
    for (int i = 0; i <= size; i++)
    {
        xEs[i] = points[i]->x;
        yEs[i] = points[i]->y;
    }

    // basic calc for line
    float a = cov(xEs, yEs, size)/var(xEs, size);
    float b = avg(yEs, size) - a * avg(xEs, size);
    Line line(a, b);

    delete[] xEs;
    delete[] yEs;

    return line;
}

Line linear_reg(const float * xEs, const float *yEs , int size)
{
    // basic calc for line
    float a = cov(xEs, yEs, size)/var(xEs, size);
    float b = avg(yEs, size) - a * avg(xEs, size);
    Line line(a, b);

    return line;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size)
{
    Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p, Line l)
{
    float fx = l.f(p.x);
    return fabs(fx - p.y);
}

// float distance(Point p1, Point p2) {
//     float Xdiff = p1.x - p2.x;
//     float Ydiff = p1.y - p2.y;
//     return sqrtf((Xdiff * Xdiff) + (Ydiff * Ydiff));
// }

