#include "sobel.h"
#include <cmath>

double magnitude(double dX, double dY)
{
    return sqrt(dX * dX + dY * dY);
}

double direction(double dX, double dY)
{
    return atan2(dY, dX);
}

void sobel_filter(Image &img)

{
    double dX = 0, dY = 0;

    // Apply the kernel to each pixel of the image
    for (int i = 1; i < img.height - 1; i++)
    {
        for (int j = 1; j < img.width - 1; j++)
        {
            dX = 0;
            dY = 0;
            // Convolve the kernel with the 3x3 pixel neighborhood
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {

                    dX += sobel_kernelX[k + 1][l + 1] * img.pixels[i + k][j + l].gray.value; // for gradient along x
                    dY += sobel_kernelY[k + 1][l + 1] * img.pixels[i + k][j + l].gray.value; // for gradient along y
                }
            }

            // calculate magnitude
            img.pixels[i][j].gray.mag = magnitude(dX, dY);

            // calculate direction
            img.pixels[i][j].gray.mag = direction(dX, dY);
        }
    }
}