#include "sobel.h"
#include <cmath>

double magnitude(double dX, double dY)
{
    return sqrt(dX * dX + dY * dY);
}

double direction(double dX, double dY)
{
    return atan2(dY, dX); // range [-pi,pi] radians
}

void sobel_filter(Image &img)

{
    // Allocate a new image to store the result image
    Image gradient_img;
    gradient_img.width = img.width;
    gradient_img.height = img.height;
    gradient_img.pixels.resize(img.height, std::vector<Pixel>(img.width));
    gradient_img.grads.resize(img.height, std::vector<gradient>(img.width));
    gradient_img.max_grad = 0;

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel
    padd_image(img, padd_size);

    double dX = 0, dY = 0;


    #pragma omp parallel for schedule(dynamic)
    // Apply the kernel to each pixel of the image
    for (int i = padd_size; i < img.height - padd_size; i += tile_size)
    {
        for (int j = padd_size; j < img.width - padd_size; j += )
        {

            dX = 0;
            dY = 0;
            // copy pixel value
            gradient_img.pixels[i - 1][j - 1].gray.value = img.pixels[i][j].gray.value;
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
            gradient_img.grads[i - 1][j - 1].mag = magnitude(dX, dY);
            gradient_img.pixels[i - 1][j - 1].gray.value = static_cast<unsigned char>(magnitude(dX, dY));
            if (gradient_img.max_grad < gradient_img.grads[i - 1][j - 1].mag)
            {
                gradient_img.max_grad = gradient_img.grads[i - 1][j - 1].mag;
            }

            // calculate direction
            gradient_img.grads[i - 1][j - 1].dir = direction(dX, dY);
        }
    }

    // Copy the  image back to the original image
    img = gradient_img;
}