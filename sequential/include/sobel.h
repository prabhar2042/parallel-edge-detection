#ifndef __SOBEL_H__
#define __SOBEL_H__

#include "image.h"

// Sobel kernels
const std::vector<std::vector<double>> sobel_kernelX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const std::vector<std::vector<double>> sobel_kernelY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

double magnitude(double dX, double dY);
double direction(double dX, double dY);
void sobel_filter(Image &img);

#endif