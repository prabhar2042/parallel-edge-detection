#ifndef __CANNY_H__
#define __CANNY_H__

#include "ppm.h"
#include "blur.h"
#include "sobel.h"
#include "nms.h"
#include "thresholding.h"
#include "edgetracking.h"

struct exec_time
{
    double rgb_to_gray;
    double gaussian_blur;
    double sobel;
    double nms;
    double double_thres;
    double edge_track;
    double total;
};

void canny_edge_detector(exec_time &time, char *read_file, char *write_file);
void print_time(exec_time time);
void print_time_debug(exec_time time);

#endif