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

enum bin_category
{
    no_edge = 0,
    weak = 1,
    strong = 2,
};

struct gradient
{
    double mag;
    double dir;

    bin_category thres_bin;
};

struct Image
{
    int width;
    int height;
    double max_grad;
    Pixel[][] pixels;
    gradient[][] grads;
};

void rgb_to_gray(Image &img);

#endif