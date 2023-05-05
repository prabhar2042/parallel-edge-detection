
#include <omp.h>
#include "thresholding.h"

#define TILE_SIZE 64

void double_thresholding(Image &img)
{
    double high_thres = img.max_grad * high_threshold_ratio;
    double low_thres = img.max_grad * low_threshold_ratio;

    //#pragma omp parallel for schedule(static)

    for (int i = 0; i < img.width - TILE_SIZE; i += TILE_SIZE)
    {
        for (int j = 0; j < img.height - TILE_SIZE; j += TILE_SIZE)
        {
            for (int ii = i; ii < TILE_SIZE; ii++)
            {
                for (int jj= j; jj < TILE_SIZE; jj++)
                {
                    if (img.grads[ii][jj].mag >= high_thres)
                    {
                        img.grads[ii][jj].thres_bin = strong;
                    }
                    else if ((img.grads[ii][jj].mag >= low_thres) && (img.grads[ii][jj].mag < high_thres))
                    {
                        img.grads[ii][jj].thres_bin = weak;
                    }
                    else
                    {
                        img.grads[ii][jj].thres_bin = no_edge;
                        img.grads[ii][jj].mag = 0;
                        img.pixels[ii][jj].gray.value = 0;
                    }

                }
            }
        }
    }


    /* for (int ij = 0; ij < img.height * img.width; ij++)
    {
        int i = ij / img.width;
        int j = ij % img.width;

            if (img.grads[i][j].mag >= high_thres)
            {
                img.grads[i][j].thres_bin = strong;
            }
            else if ((img.grads[i][j].mag >= low_thres) && (img.grads[i][j].mag < high_thres))
            {
                img.grads[i][j].thres_bin = weak;
            }
            else
            {
                img.grads[i][j].thres_bin = no_edge;
                img.grads[i][j].mag = 0;
                img.pixels[i][j].gray.value = 0;
            }
    } */
}
