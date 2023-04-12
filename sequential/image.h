#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>

#include "error.h"

union Pixel
{
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    } rgb;

    struct
    {
        unsigned char value;
    } gray;
};

struct Image
{
    int width;
    int height;
    std::vector<std::vector<Pixel>> pixels;
};

void rgb_to_gray(Image &img);

#endif