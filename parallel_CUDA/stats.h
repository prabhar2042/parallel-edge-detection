#ifndef __STATS_H__
#define __STATS_H__

#include <time.h>
#include <stdio.h>

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

void print_time(exec_time time);
void print_time_debug(exec_time time);

#endif