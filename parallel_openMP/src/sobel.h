#ifndef __SOBEL_H__
#define __SOBEL_H__

#include "image.h"

#define TILE_SIZE 32

// Sobel kernels
const std::vector<std::vector<double>> sobel_kernelX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const std::vector<std::vector<double>> sobel_kernelY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


double magnitude(double dX, double dY);
double direction(double dX, double dY);
void sobel_filter(Image &img);
void sobel_filter_tiled(Image &img, int tile_size);

#endif