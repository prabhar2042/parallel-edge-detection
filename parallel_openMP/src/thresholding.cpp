

#include "thresholding.h"

void double_thresholding(Image &img)
{
    double high_thres = img.max_grad * high_threshold_ratio;
    double low_thres = img.max_grad * low_threshold_ratio;

    for (int i = 0; i < img.height; i++)
    {
        for (int j = 0; j < img.width; j++)
        {

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
}
