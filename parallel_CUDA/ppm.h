#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <fstream>
#include "image.h"

void read_PPM(Image &img, const char *filename);

void write_PPM(const Image &img, const char *filename);

void flatten_pixel_array(Image img, Pixel *flat_pixels);

void unflatten_pixel_array(Image img, Pixel *flat_pixels);

#endif