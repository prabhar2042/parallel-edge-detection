
#include <omp.h>
#include "thresholding.h"

void double_thresholding(Image &img)
{
    double high_thres = img.max_grad * high_threshold_ratio;
    double low_thres = img.max_grad * low_threshold_ratio;

    #pragma omp parallel for schedule(static,1)
        for (int ij = 0; ij < img.height * img.width; ij++)
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
        }
}
