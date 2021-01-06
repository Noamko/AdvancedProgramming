// Noam koren (308192871)	Nir birnboim (313194672)

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "anomaly_detection_util.h"
using namespace std;

/**
 * @brief Circle class
 * used to create circles :)
 */
class Circle {
public:
    Circle();
    Circle(Point c, float r) : center(c), radius(r) {}
    Point center;
    float radius;
};

Circle bestCircle(Point **points, vector<Point> R, size_t size);

Circle build(vector<Point> &points);

Circle createFrom3Points(Point &p1, Point &p2, Point &p3);

Circle createFrom2Points(Point &p1, Point &p2);

Circle findMinCircle(Point **points, size_t size);

int isPointInCircle(Point &point1, Circle &c);

float dist2Points(const Point &p1, const Point &p2);

void swap(Point &p1, Point &p2);

void shuffle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
