
#include "nms.h"
#include <stdio.h> // debug
#include <time.h>  // debug

void non_max_suppression(Image &img)
{
    int total_threads = 8;
    double start_time[total_threads];
    double total[total_threads];

#pragma omp parallel for schedule(dynamic)

    for (int i = 1; i < img.height - 1; i++)
    {
        int tid = omp_get_thread_num();
        start_time[tid] = clock();

        for (int j = 1; j < img.width - 1; j++)
        {
            double mag = img.grads[i][j].mag;
            double dir = img.grads[i][j].dir;

            // Determine adjacent pixels based on gradient direction
            double pixel1_mag, pixel2_mag;
            if ((dir > -PI_BY_8 && dir <= PI_BY_8) || (dir > 7 * PI_BY_8 && dir <= -7 * PI_BY_8))
            {
                pixel1_mag = img.grads[i][j - 1].mag;
                pixel2_mag = img.grads[i][j + 1].mag;
            }
            else if ((dir > PI_BY_8 && dir <= 3 * PI_BY_8) || (dir > -7 * PI_BY_8 && dir <= -5 * PI_BY_8))
            {
                pixel1_mag = img.grads[i - 1][j + 1].mag;
                pixel2_mag = img.grads[i + 1][j - 1].mag;
            }
            else if ((dir > 3 * PI_BY_8 && dir <= 5 * PI_BY_8) || (dir > -5 * PI_BY_8 && dir <= -3 * PI_BY_8))
            {
                pixel1_mag = img.grads[i - 1][j].mag;
                pixel2_mag = img.grads[i + 1][j].mag;
            }
            else
            {
                pixel1_mag = img.grads[i - 1][j - 1].mag;
                pixel2_mag = img.grads[i + 1][j + 1].mag;
            }

            // Suppress non-maximum pixels
            if (mag <= pixel1_mag || mag <= pixel2_mag)
            {
                img.grads[i][j].mag = 0;
                img.pixels[i][j].gray.value = 0;
            }
        }
        total[tid] = ((double)clock() - (double)start_time[tid]) / CLOCKS_PER_SEC;
    }

    for (int i = 0; i < total_threads; i++)
    {
        printf("tid : %d, time : %f\n", i, total[i]);
    }
}

void non_max_suppression_tiled(Image &img, int tile_size)
{

#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i < img.height - tile_size - 1; i += tile_size)
    {
        for (int j = 1; j < img.width - tile_size - 1; j += tile_size)
        {
            for (int ii = i; ii < i + tile_size; ii++)
            {
                for (int jj = j; jj < j + tile_size; jj++)
                {
                    double mag = img.grads[ii][jj].mag;
                    double dir = img.grads[ii][jj].dir;

                    // Determine adjacent pixels based on gradient direction
                    double pixel1_mag, pixel2_mag;
                    if ((dir > -PI_BY_8 && dir <= PI_BY_8) || (dir > 7 * PI_BY_8 && dir <= -7 * PI_BY_8))
                    {
                        pixel1_mag = img.grads[ii][jj - 1].mag;
                        pixel2_mag = img.grads[ii][jj + 1].mag;
                    }
                    else if ((dir > PI_BY_8 && dir <= 3 * PI_BY_8) || (dir > -7 * PI_BY_8 && dir <= -5 * PI_BY_8))
                    {
                        pixel1_mag = img.grads[ii - 1][jj + 1].mag;
                        pixel2_mag = img.grads[ii + 1][jj - 1].mag;
                    }
                    else if ((dir > 3 * PI_BY_8 && dir <= 5 * PI_BY_8) || (dir > -5 * PI_BY_8 && dir <= -3 * PI_BY_8))
                    {
                        pixel1_mag = img.grads[ii - 1][jj].mag;
                        pixel2_mag = img.grads[ii + 1][jj].mag;
                    }
                    else
                    {
                        pixel1_mag = img.grads[ii - 1][jj - 1].mag;
                        pixel2_mag = img.grads[ii + 1][jj + 1].mag;
                    }

                    // Suppress non-maximum pixels
                    if (mag <= pixel1_mag || mag <= pixel2_mag)
                    {
                        img.grads[ii][jj].mag = 0;
                        img.pixels[ii][jj].gray.value = 0;
                    }
                }
            }
        }
    }
}