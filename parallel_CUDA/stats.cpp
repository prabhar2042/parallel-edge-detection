#include "stats.h"

void print_time(exec_time time)
{

    printf("rgb_to_gray : %f\n", time.rgb_to_gray);

    printf("gaussian_blur : %f\n", time.gaussian_blur);

    printf("sobel filter : %f\n", time.sobel);

    printf("non_maximum_suppression : %f\n", time.nms);

    printf("double thresholding : %f\n", time.double_thres);

    printf("edge tracking  : %f\n", time.edge_track);

    printf("canny edge detecion  : %f\n", time.total);
}

void print_time_debug(exec_time time)
{

    printf("rgb_to_gray : %f total time % : %f\n", time.rgb_to_gray, (time.rgb_to_gray / time.total) * 100);

    printf("gaussian_blur : %f total time % : %f\n", time.gaussian_blur, (time.gaussian_blur / time.total) * 100);

    printf("sobel filter : %f total time % : %f\n", time.sobel, (time.sobel / time.total) * 100);

    printf("non_maximum_suppression : %f total time % : %f\n", time.nms, (time.nms / time.total) * 100);

    printf("double thresholding : %f total time % : %f\n", time.double_thres, (time.double_thres / time.total) * 100);

    printf("edge tracking  : %f total time % : %f\n", time.edge_track, (time.edge_track / time.total) * 100);

    printf("canny edge detecion  : %f\n total time % : %f\n", time.total, (time.rgb_to_gray / time.total) * 100);
}