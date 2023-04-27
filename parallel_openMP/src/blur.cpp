#include "blur.h"
#include <stdio.h> // debug
#include <time.h>  // debug
#include <cmath>

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

    //
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
    }

    // Copy the blurred image back to the original image
    img = blurredImg;
}

void gaussian_Blur_tiled(Image &img, int tile_size)
{

    // Allocate a new image to store the blurred image
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel
    padd_image(img, padd_size);

    int num_tiles_y = std::ceil((float)img.height / tile_size);
    int num_tiles_x = std::ceil((float)img.width / tile_size);

// Apply the kernel to each tile of the image
#pragma omp parallel for schedule(dynamic)
    for (int tile_y = 0; tile_y < num_tiles_y; ++tile_y)
    {
        for (int tile_x = 0; tile_x < num_tiles_x; ++tile_x)
        {
            int start_row = tile_y * tile_size + padd_size;
            int end_row = std::min(start_row + tile_size, img.height - padd_size);
            int start_col = tile_x * tile_size + padd_size;
            int end_col = std::min(start_col + tile_size, img.width - padd_size);

            // Convolve the kernel with each pixel of the tile
            for (int row = start_row; row < end_row; ++row)
            {
                for (int col = start_col; col < end_col; ++col)
                {
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
            }
        }
    }

    // Copy the blurred image back to the original image
    img = blurredImg;
}

void gaussian_Blur_seperable(Image &img)
{
    // Allocate a new image to store the blurred image
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel
    padd_image(img, padd_size);

    // Allocate a temporary image to store the result of horizontal convolution
    std::vector<std::vector<float>> tempImg;

    tempImg.resize(img.height, std::vector<float>(img.width));

// Apply the horizontal kernel to each row of the image
#pragma omp parallel for schedule(dynamic)
    for (int row = padd_size; row < img.height - padd_size; ++row)
    {
        for (int col = padd_size; col < img.width - padd_size; ++col)
        {
            // Convolve the kernel with the 3x1 pixel neighborhood
            float sum = 0.0;
            for (int i = -1; i <= 1; ++i)
            {
                sum += blur_kernel_hor[i + 1] * img.pixels[row][col + i].gray.value;
            }
            // Store the blurred pixel in the temporary image
            tempImg[row][col] = sum;
        }
    }

// Apply the vertical kernel to each column of the temporary image
#pragma omp parallel for schedule(dynamic)
    for (int col = padd_size; col < img.width - padd_size; ++col)
    {
        for (int row = padd_size; row < img.height - padd_size; ++row)
        {
            // Convolve the kernel with the 1x3 pixel neighborhood
            float sum = 0.0;
            for (int i = -1; i <= 1; ++i)
            {
                sum += blur_kernel_vert[i + 1] * tempImg[row + i][col];
            }
            // Store the blurred pixel in the new image
            blurredImg.pixels[row - 1][col - 1].gray.value = static_cast<unsigned char>(sum);
        }
    }

    // Copy the blurred image back to the original image
    img = blurredImg;
}

void gaussian_Blur_seperable_tiled(Image &img, int tile_size)
{
    // Allocate a new image to store the blurred image
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // padding the input image to insure same size
    int padd_size = 1; // for 3X3 kernel
    padd_image(img, padd_size);

    int num_tiles_y = std::ceil((float)img.height / tile_size);
    int num_tiles_x = std::ceil((float)img.width / tile_size);

    // Allocate a temporary image to store the result of horizontal convolution
    std::vector<std::vector<float>> tempImg;
    tempImg.resize(img.height, std::vector<float>(img.width));

#pragma omp parallel for schedule(dynamic)
    for (int tile_y = 0; tile_y < num_tiles_y; ++tile_y)
    {
        for (int tile_x = 0; tile_x < num_tiles_x; ++tile_x)
        {
            int start_row = tile_y * tile_size + padd_size;
            int end_row = std::min(start_row + tile_size, img.height - padd_size);
            int start_col = tile_x * tile_size + padd_size;
            int end_col = std::min(start_col + tile_size, img.width - padd_size);

            // Apply the horizontal kernel to each row of the tile
            for (int row = start_row; row < end_row; ++row)
            {
                for (int col = start_col; col < end_col; ++col)
                {
                    // Convolve the kernel with the 3x1 pixel neighborhood
                    float sum = 0.0;
                    for (int i = -1; i <= 1; ++i)
                    {
                        sum += blur_kernel_hor[i + 1] * img.pixels[row][col + i].gray.value;
                    }
                    // Store the blurred pixel in the temporary image
                    tempImg[row][col] = sum;
                }
            }

            // Apply the vertical kernel to each column of the tile

            for (int row = start_row; row < end_row; ++row)
            {
                for (int col = start_col; col < end_col; ++col)
                {
                    // Convolve the kernel with the 1x3 pixel neighborhood
                    float sum = 0.0;
                    for (int i = -1; i <= 1; ++i)
                    {
                        sum += blur_kernel_vert[i + 1] * tempImg[row + i][col];
                    }
                    // Store the blurred pixel in the new image
                    blurredImg.pixels[row - 1][col - 1].gray.value = static_cast<unsigned char>(sum);
                }
            }
        }
    }

    // Copy the blurred image back to the original image
    img = blurredImg;
}



