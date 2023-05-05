#include "canny.h"
#include <time.h>

void canny_edge_detector(exec_time &time, char *read_file, char *write_file)
{
    omp_set_num_threads(8);

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
    // gaussian_Blur(img); // 2. Gaussian Blur TODO: Krish
    gaussian_Blur_tiled(img, TILE_SIZE_BLUR);
    // gaussian_Blur_seperable(img);
    // gaussian_Blur_seperable_tiled(img, TILE_SIZE_BLUR);
    end = clock();
    time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.gaussian_blur;

    start = clock();
    // sobel_filter(img); // 3. Determine intensity gradient
    sobel_filter_no_padd(img);
    end = clock();
    time.sobel = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.sobel;

    start = clock();
    // non_max_suppression(img);
    non_max_suppression_tiled(img, TILE_SIZE); // 4. Non Maximum seperation
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