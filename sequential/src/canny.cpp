#include "canny.h"
#include <time.h>

#include <iostream>
#include <fstream>
using namespace std;

void canny_edge_detector(exec_time &time, char *read_file, char *write_file)
{

    clock_t start, end;
    time.total = 0;
    Image img;

    // read image
    read_PPM(img, read_file);

    start = clock();
    rgb_to_gray(img); // 1. convert image to grayscale TODO: Krish
    end = clock();
    time.rgb_to_gray = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.rgb_to_gray;


    start = clock();
    gaussian_Blur(img); // 2. Gaussian Blur TODO: Krish
    end = clock();
    time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.gaussian_blur;


    // print values here
    ofstream myfile;
    myfile.open ("log.txt");

    myfile << "Gaussian\n";

    for (int i = 0; i < (int)img.pixels.size(); i++)
    {
        for (int j = 0; j < (int)img.pixels[i].size(); j++)
        {
            Pixel p = img.pixels[i][j];
            myfile << "Gray: " << (int) p.gray.value << "\n";
        }
    }

    myfile.close();

    start = clock();
    sobel_filter(img); // 3. Determine intensity gradient
    end = clock();
    time.sobel = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.sobel;


    start = clock();
    non_max_suppression(img); // 4. Non Maximum seperation TODO: Krish
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