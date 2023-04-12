#include <iostream>
#include "ppm.h"
#include "blur.h"
#include "sobel.h"

int main(int argc, char *argv[])
{
    printf("start \n");

    char read_file[] = "dataset/sample_640×426.ppm";
    char writ_file[] = "dataset/test.ppm";

    Image img;

    // read image
    read_PPM(img, read_file);

    rgb_to_gray(img);   // 1. convert image to grayscale
    gaussian_Blur(img); // 2. Gaussian Blur
    sobel_filter(img);  // 3. Determine intensity gradient
    write_PPM(img, writ_file);

    printf("done \n");
}
