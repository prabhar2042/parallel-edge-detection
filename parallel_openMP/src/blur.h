#ifndef __BLUR_H__
#define __BLUR_H__

#include "image.h"
#define TILE_SIZE_BLUR 32

// Define the 3x3 Gaussian kernel
const float blur_kernel[3][3] = {{0.0625, 0.125, 0.0625},
                                 {0.125, 0.25, 0.125},
                                 {0.0625, 0.125, 0.0625}};

const float blur_kernel_hor[3] = {0.0625, 0.125, 0.0625};
const float blur_kernel_vert[3] = {1, 2, 1};

void gaussian_Blur(Image &img);
void gaussian_Blur_tiled(Image &img, int tile_size);
void gaussian_Blur_seperable(Image &img);
void gaussian_Blur_seperable_tiled(Image &img, int tile_size);

#endif