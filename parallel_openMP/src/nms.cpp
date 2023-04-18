
#include "nms.h"

void non_max_suppression(Image &img)
{
#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i < img.height - 1; i++)
    {
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
    }
}