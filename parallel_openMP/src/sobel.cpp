#include "sobel.h"
#include <cmath>

inline double magnitude(double dX, double dY)
{
    return sqrt(dX * dX + dY * dY);
}

inline double direction(double dX, double dY)
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

    // Apply the kernel to each pixel of the image
    for (int i = padd_size; i < img.height - padd_size; i++)
    {
        for (int j = padd_size; j < img.width - padd_size; j++)
        {
            dX = 0;
            dY = 0;
            // copy pixel value
            gradient_img.pixels[i - 1][j - 1].gray.value = img.pixels[i][j].gray.value;
// Convolve the kernel with the 3x3 pixel neighborhood
#pragma omp simd
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

void sobel_filter_tiled(Image &img, int tile_size)

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

// Apply the kernel to each tile of the image
#pragma omp parallel for schedule(dynamic)
    // Apply the kernel to each pixel of the image
    for (int i = padd_size; i < img.height - tile_size - padd_size; i += tile_size)
    {
        for (int j = padd_size; j < img.width - tile_size - padd_size; j += tile_size)
        {
            for (int ii = i; ii < i + tile_size; ii++)
            {
                for (int jj = j; jj < j + tile_size; jj++)
                {

                    dX = 0;
                    dY = 0;
                    // copy pixel value
                    gradient_img.pixels[ii - 1][jj - 1].gray.value = img.pixels[ii][jj].gray.value;
                    // Convolve the kernel with the 3x3 pixel neighborhood
                    for (int k = -1; k <= 1; k++)
                    {
                        for (int l = -1; l <= 1; l++)
                        {

                            dX += sobel_kernelX[k + 1][l + 1] * img.pixels[ii + k][jj + l].gray.value; // for gradient along x
                            dY += sobel_kernelY[k + 1][l + 1] * img.pixels[ii + k][jj + l].gray.value; // for gradient along y
                        }
                    }

                    // calculate magnitude
                    gradient_img.grads[ii - 1][jj - 1].mag = magnitude(dX, dY);
                    gradient_img.pixels[ii - 1][jj - 1].gray.value = static_cast<unsigned char>(magnitude(dX, dY));
                    if (gradient_img.max_grad < gradient_img.grads[ii - 1][jj - 1].mag)
                    {
                        gradient_img.max_grad = gradient_img.grads[ii - 1][jj - 1].mag;
                    }

                    // calculate direction
                    gradient_img.grads[ii - 1][jj - 1].dir = direction(dX, dY);
                }
            }
        }
    }

    // Copy the  image back to the original image
    img = gradient_img;
}

void sobel_filter_no_padd(Image &img)
{

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel

    img.grads.resize(img.height, std::vector<gradient>(img.width));
    img.max_grad = 0;
    // padd_image(img, padd_size);

    double dX = 0, dY = 0;

    // Apply the kernel to each pixel of the image
    for (int i = padd_size; i < img.height - padd_size; i++)
    {
        for (int j = padd_size; j < img.width - padd_size; j++)
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
            img.grads[i][j].mag = magnitude(dX, dY);

            if (img.max_grad < img.grads[i][j].mag)
            {
                img.max_grad = img.grads[i][j].mag;
            }

            // calculate direction
            img.grads[i][j].dir = direction(dX, dY);
        }
    }

// update pixel values
// Apply the kernel to each pixel of the image
#pragma omp parallel for schedule(static)
    for (int i = padd_size; i < img.height - padd_size; i++)
    {

        for (int j = padd_size; j < img.width - padd_size; j++)
        {

            img.pixels[i][j].gray.value = static_cast<unsigned char>(img.grads[i][j].mag);
        }
    }
}

void sobel_filter_no_pad_tiled(Image &img, int tile_size)
{
    int padd_size = 1; // for 3x3 kernel

    img.grads.resize(img.height, std::vector<gradient>(img.width));
    img.max_grad = 0;

    // Apply the kernel to each tile of the image
    for (int tile_y = 0; tile_y < img.height; tile_y += tile_size)
    {
        for (int tile_x = 0; tile_x < img.width; tile_x += tile_size)
        {
            int end_row = std::min(tile_y + tile_size, img.height);
            int end_col = std::min(tile_x + tile_size, img.width);

            // Convolve the kernel with each pixel of the tile
            for (int i = tile_y + padd_size; i < end_row - padd_size; ++i)
            {
                for (int j = tile_x + padd_size; j < end_col - padd_size; ++j)
                {
                    double dX = 0.0;
                    double dY = 0.0;

                    // Convolve the kernel with the 3x3 pixel neighborhood
                    for (int k = -1; k <= 1; ++k)
                    {
                        for (int l = -1; l <= 1; ++l)
                        {
                            dX += sobel_kernelX[k + 1][l + 1] * img.pixels[i + k][j + l].gray.value; // for gradient along x
                            dY += sobel_kernelY[k + 1][l + 1] * img.pixels[i + k][j + l].gray.value; // for gradient along y
                        }
                    }

                    // calculate magnitude
                    img.grads[i][j].mag = magnitude(dX, dY);
                    if (img.max_grad < img.grads[i][j].mag)
                    {
                        img.max_grad = img.grads[i][j].mag;
                    }

                    // calculate direction
                    img.grads[i][j].dir = direction(dX, dY);
                }
            }
        }
    }

    // Update pixel values
    for (int tile_y = 0; tile_y < img.height; tile_y += tile_size)
    {
        for (int tile_x = 0; tile_x < img.width; tile_x += tile_size)
        {
            int end_row = std::min(tile_y + tile_size, img.height);
            int end_col = std::min(tile_x + tile_size, img.width);

            // Update the pixel values within each tile
            for (int i = tile_y + padd_size; i < end_row - padd_size; ++i)
            {
                for (int j = tile_x + padd_size; j < end_col - padd_size; ++j)
                {
                    img.pixels[i][j].gray.value = static_cast<unsigned char>(img.grads[i][j].mag);
                }
            }
        }
    }
}