#ifndef __NMS_H__
#define __NMS_H__

#include "image.h"

#define TILE_SIZE 64

const double PI_BY_8 = 0.785398163397448309616 / 2;
void non_max_suppression(Image &img);
void non_max_suppression_tiled(Image &img, int tile_size);

#endif