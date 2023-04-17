#ifndef __BLUR_H__
#define __BLUR_H__

#include "image.h"

// Define the 3x3 Gaussian kernel
const float blur_kernel[3][3] = {{0.0625, 0.125, 0.0625},
                                 {0.125, 0.25, 0.125},
                                 {0.0625, 0.125, 0.0625}};

void gaussian_Blur(Image &img);

#endif