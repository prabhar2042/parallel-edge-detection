#include "blur.h"
#include <stdio.h> // debug
#include <time.h>  // debug

void gaussian_Blur(Image &img)
{

    // Allocate a new image to store the blurred image
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel
    //padd_image(img, padd_size);

    // int total_threads = 8;            // debug
    // double start_time[total_threads]; // debug
    // double total[total_threads];      // debug

    // #pragma omp parallel for schedule(dynamic)
    //  Apply the kernel to each pixel of the image
    for (int row = padd_size; row < img.height - padd_size; ++row)
    {
        // int tid = omp_get_thread_num(); // debug
        // start_time[tid] = clock();      // debug
        for (int col = padd_size; col < img.width - padd_size; ++col)
        {
            // Convolve the kernel with the 3x3 pixel neighborhood
            float sum = 0.0;
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    sum += blur_kernel[i + 1][j + 1] * img.pixels[row + i][col + j].gray.value;
                }
            }
            // Store the blurred pixel in the new image
            blurredImg.pixels[row - 1][col - 1].gray.value = static_cast<unsigned char>(sum);
        }
        // total[tid] = ((double)clock() - (double)start_time[tid]) / CLOCKS_PER_SEC; // debug
    }
    // Debug
    /*for (int i = 0; i < total_threads; i++)
    {
        printf("tid : %d, time : %f\n", i, total[i]);
    }
    // Debug*/

    // Copy the blurred image back to the original image
    img = blurredImg;
}



