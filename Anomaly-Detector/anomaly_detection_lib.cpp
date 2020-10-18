#include "anomaly_detection_lib.h"
#include <cmath>

//TODO:
/*
 check mikrei kaze
 add comments
 do tests
*/
float mean(float* x, int size) {
	if (x == NULL) {
		throw "ERROR: Null pointer was given!";
	}

	if(size <= 0) {
		throw "ERROR: size cant be less or equal to zero";
	}

	float mean = 0;
	for (int i = 0; i < size; i++) {
		mean += *(x + i);
	}
	mean /= size;
	return mean;
}

float var(float* x, int size){
	float u = 0;
	float variance = 0;
	for(int i = 0; i < size; i++) {
		u += *(x + i);
		variance += (*(x + i)) * (*(x + i));
	}
	u /= size;
	variance = variance/size - u*u;
	return variance;
}

float cov(float* x, float* y, int size) {
	float mean_x = mean(x,size);
	float mean_y = mean(y,size);
	float covariance = 0;
	for (int i = 0; i < size; i++) {
		covariance += (*(x + i) - mean_x) * (*(y + i) - mean_y);
	}	
	covariance /= size;
	return covariance;
}

float pearson(float* x, float* y, int size) {
	float sigx = std::sqrt(var(x, size));
	float sigy = std::sqrt(var(y, size));
	return cov(x, y, size) / (sigx * sigy);
}

Line linear_reg(Point** points, int size) {
	float x_values[size];
	float y_values[size];
	for (int i = 0; i < size; i++) {
		x_values[i] = points[i]->x;
		y_values[i] = points[i]->y;
	}
	float a = cov(x_values, y_values, size) / var(x_values, size);
	float b = mean(y_values, size) - a * mean(x_values, size);

	return Line(a, b);
}