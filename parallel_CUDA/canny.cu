#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include "canny.h"

#define THREADS_PER_BLOCK 16

__constant__ float blur_kernel[3][3] = {{0.0625, 0.125, 0.0625},
                                        {0.125, 0.25, 0.125},
                                        {0.0625, 0.125, 0.0625}};

__global__ void rgb_to_gray_kernel(Pixels *pixels, int height, int width)
{

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    double grayValue = 0.299 * pixels[row * width + col].rgb.r +
                       0.587 * pixels[row * width + col].rgb.g +
                       0.114 * pixels[row * width + col].rgb.b;

    // pixels[row * width + col].gray.value = static_cast<unsigned char>(grayValue);
    pixels[row * width + col].gray.value = 0;
}

__global__ void gaussianblur_kernel(Image device_img, Image device_result)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row > (device_img.height - 1) || col > (device_img.width - 1) || row < 1 || col < 1)
    {
        return;
    }

    float sum = 0.0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            sum += blur_kernel[i + 1][j + 1] * device_img.pixels[row + i][col + j].gray.value;
        }
    }
    // Store the blurred pixel in the new image
    device_result.pixels[row][col].gray.value = static_cast<unsigned char>(sum);
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
    // allocate memory for host image on stack
    Image host_img;

    printf("about to read ppm\n");

    // read image
    read_PPM(host_img, read_file);

    printf("read done \n");

    // flatten Pixel array
    Pixel host_flat_pixels[host_img.height * host_img.width];
    flatten_array(host_img, host_flat_pixels);

    // allocate memory for image on GPU
    Pixel *device_pixels;
    cudaMalloc((void **)&device_pixels, host_img.width * host_img.height * sizeof(Pixel));
    printf("malloc 1 done  \n");

    // copy image to GPU
    cudaMemcpy(device_pixels, host_flat_pixels, host_img.width * host_img.height * sizeof(Pixel), cudaMemcpyHostToDevice);

    printf("memcpy done  \n");

    // Define block and grid dimensions
    dim3 blockDim(THREADS_PER_BLOCK, THREADS_PER_BLOCK);
    dim3 gridDim((host_img.width + blockDim.x - 1) / blockDim.x,
                 (host_img.height + blockDim.y - 1) / blockDim.y);

    double start, end;

    // start image detection on GPU

    start = clock();
    rgb_to_gray_kernel<<<blockDim, gridDim>>>(device_pixels, host_img.height, host_img.width); // 1. convert image to grayscale
    cudaDeviceSynchronize();
    end = clock();
    time.rgb_to_gray = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.rgb_to_gray;

    /*
        start = clock();
        gaussianblur_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result); // 2. Gaussian Blur
        cudaDeviceSynchronize();
        end = clock();
        time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
        time.total += time.gaussian_blur;

        start = clock();
        sobel_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result); // 3. Determine intensity gradient
        cudaDeviceSynchronize();
        end = clock();
        time.sobel = ((double)end - (double)start) / CLOCKS_PER_SEC;
        time.total += time.sobel;

        start = clock();
        nms_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result); // 4. Non Maximum seperation
        cudaDeviceSynchronize();
        end = clock();
        time.nms = ((double)end - (double)start) / CLOCKS_PER_SEC;
        time.total += time.nms;

        start = clock();
        threshholding_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result); // 5. Double Thresholding
        cudaDeviceSynchronize();
        end = clock();
        time.double_thres = ((double)end - (double)start) / CLOCKS_PER_SEC;
        time.total += time.double_thres;

        start = clock();
        edge_tracking_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result); // 6. Edge Tracking
        cudaDeviceSynchronize();
        end = clock();
        time.edge_track = ((double)end - (double)start) / CLOCKS_PER_SEC;
        time.total += time.edge_track;

        */

    // Copy image data from device to host

    printf("I am here\n");
    cudaMemcpy(host_flat_pixels, device_pixels, host_img.width * host_img.height * sizeof(Pixel), cudaMemcpyDeviceToHost);

    unflatten_pixel_array(host_img, host_flat_pixels);

    // write image
    write_PPM(host_img, write_file);

    // free device memory space
    cudaFree(device_pixels);
}
