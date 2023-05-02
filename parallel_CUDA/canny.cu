#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include "canny.h"

typedef uchar3 Pixel_cuda;

#define THREADS_PER_BLOCK 16

__constant__ float blur_kernel[3][3] = {{0.0625, 0.125, 0.0625},
                                        {0.125, 0.25, 0.125},
                                        {0.0625, 0.125, 0.0625}};

void flatten_pixel_array(Image img, Pixel_cuda *flat_pixels)
{

    for (int i = 0; i < img.height; i++)
    {
        for (int j = 0; j < img.width; j++)
        {
            flat_pixels[i * img.width + j].x = img.pixels[i][j].rgb.r;
            flat_pixels[i * img.width + j].y = img.pixels[i][j].rgb.g;
            flat_pixels[i * img.width + j].z = img.pixels[i][j].rgb.b;
        }
    }
}

void unflatten_pixel_array(Image img, unsigned char *flat_pixels)
{
    for (int i = 0; i < img.height; i++)
    {
        for (int j = 0; j < img.width; j++)
        {
            img.pixels[i][j].gray.value = static_cast<unsigned char>(flat_pixels[i * img.width + j]);
        }
    }
}

__global__ void rgb_to_gray_kernel(Pixel_cuda *pixels, unsigned char *output, int height, int width)
{

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    uchar3 p = pixels[row * width + col];

    double grayValue = (0.299 * p.x) +
                       (0.587 * p.y) +
                       (0.114 * p.z);

    output[row * width + col] = static_cast<unsigned char>(grayValue);
    // output[row * width + col] = static_cast<unsigned char>(100);
}

__global__ void gaussianblur_kernel(unsigned char *gray, unsigned char *output, int height, int width)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    if (row < 1 || col < 1 || row >= (height - 1) || col >= (width - 1))
    {
        // Pixels at the image borders are left unchanged
        output[row * width + col] = gray[row * width + col];
        return;
    }

    float sum = 0.0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            sum += blur_kernel[i + 1][j + 1] * gray[(row + i) * width + (col + j)];
        }
    }

    output[row * width + col] = static_cast<unsigned char>(sum);
}

/*
__global__ void sobel_kernel(Image *img)
{
}

__global__ void nms_kernel(Image *img)
{
}

__global__ void threshholding_kernel(Image *img)
{
}

__global__ void edge_tracking_kernel(Image *img){}

*/

void canny(exec_time &time, char *read_file, char *write_file)
{
    // Allocate memory for host image on stack
    Image host_img;

    // Read image
    read_PPM(host_img, read_file);

    // Flatten Pixel array
    Pixel_cuda *host_flat_pixels = (Pixel_cuda *)malloc(host_img.width * host_img.height * sizeof(Pixel_cuda));
    flatten_pixel_array(host_img, host_flat_pixels);

    // Allocate memory for RGB image on GPU
    Pixel_cuda *device_pixels;
    cudaMalloc((void **)&device_pixels, host_img.width * host_img.height * sizeof(Pixel_cuda));

    // Allocate memory for grayscale image on GPU
    unsigned char *device_gray;
    cudaMalloc((void **)&device_gray, host_img.width * host_img.height * sizeof(unsigned char));

    // Allocate memory for grayscale image output on GPU
    unsigned char *device_gray_out;
    cudaMalloc((void **)&device_gray_out, host_img.width * host_img.height * sizeof(unsigned char));

    // Copy RGB image to GPU
    cudaMemcpy(device_pixels, host_flat_pixels, host_img.width * host_img.height * sizeof(Pixel_cuda), cudaMemcpyHostToDevice);

    // Define block and grid dimensions
    dim3 blockDim(THREADS_PER_BLOCK, THREADS_PER_BLOCK);
    dim3 gridDim((host_img.width + blockDim.x - 1) / blockDim.x,
                 (host_img.height + blockDim.y - 1) / blockDim.y);

    double start, end;

    // Start image detection on GPU
    printf("pixel before grey: %d\n", host_flat_pixels[0].x);

    // 1. Convert image to grayscale
    start = clock();
    rgb_to_gray_kernel<<<gridDim, blockDim>>>(device_pixels, device_gray, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.rgb_to_gray = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.rgb_to_gray;

    /*// 2. Gaussian Blur
    start = clock();
    gaussianblur_kernel<<<gridDim, blockDim>>>(device_gray, device_gray_out, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.gaussian_blur;*/

    // Copy image data from device to host
    unsigned char *host_gray = (unsigned char *)malloc(host_img.width * host_img.height * sizeof(unsigned char));
    cudaMemcpy(host_gray, device_gray, host_img.width * host_img.height * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    /*for (int i = 0; i < (host_img.width * host_img.height); i++)
    {
        printf("pixel after grey: %d\n", host_gray[i]);
        if (i > 25)
        {
            break;
        }
    }*/

    // Unflatten pixel array
    unflatten_pixel_array(host_img, host_gray);

    // Write image
    write_PPM(host_img, write_file);

    // Free device memory space
    cudaFree(device_pixels);
    cudaFree(device_gray);

    // Free host memory
    free(host_flat_pixels);
    free(host_gray);
}
