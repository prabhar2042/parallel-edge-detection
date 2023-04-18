#ifndef __THRESHOLDING_H__
#define __THRESHOLDING_H__

#include "image.h"

const double high_threshold_ratio = 0.1;
const double low_threshold_ratio = 0.05;

void double_thresholding(Image &img);

#endif