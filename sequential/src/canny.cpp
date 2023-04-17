#include "canny.h"
#include <time.h>

void canny_edge_detector(exec_time &time, char *read_file, char *write_file)
{

    clock_t start, end;
    time.total = 0;
    Image img;

    // read image
    read_PPM(img, read_file);

    start = clock();
    rgb_to_gray(img); // 1. convert image to grayscale
    end = clock();
    time.rgb_to_gray = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.rgb_to_gray;

    start = clock();
    gaussian_Blur(img); // 2. Gaussian Blur
    end = clock();
    time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.gaussian_blur;

    start = clock();
    sobel_filter(img); // 3. Determine intensity gradient
    end = clock();
    time.sobel = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.sobel;

    start = clock();
    non_max_suppression(img); // 4. Non Maximum seperation
    end = clock();
    time.nms = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.nms;

    start = clock();
    double_thresholding(img); // 5. Double Thresholding
    end = clock();
    time.double_thres = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.double_thres;

    start = clock();
    edge_tracking(img); // 6. Edge Tracking
    end = clock();
    time.edge_track = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.edge_track;

    // write image
    write_PPM(img, write_file);
}

void print_time(exec_time time)
{

    printf("Execution time for rgb_to_gray : %f\n", time.rgb_to_gray);

    printf("Execution time for gaussian_blur : %f\n", time.gaussian_blur);

    printf("Execution time for sobel filter : %f\n", time.sobel);

    printf("Execution time for non_maximum_suppression : %f\n", time.nms);

    printf("Execution time for double thresholding : %f\n", time.double_thres);

    printf("Execution time for edge tracking  : %f\n", time.edge_track);

    printf("Total execution time for canny edge detecion  : %f\n", time.total);
}