#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include <iostream>
#include <fstream>
using namespace std;

#include <cmath>
#include "canny.h"

#define THREADS_PER_BLOCK 32
#define PI_BY_8 = 0.785398163397448309616 / 2;

typedef uchar3 Pixel_cuda;
typedef float3 grad_cuda;

__device__ inline double magnitude(double dX, double dY)
{
    return sqrt(dX * dX + dY * dY);
}

__device__ inline double direction(double dX, double dY)
{
    return atan2(dY, dX); // range [-pi,pi] radians
}

double find_max_grad(grad_cuda *grad, int height, int width)
{
    double max_grad = 0;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (grad[row * width + col].x > max_grad)
            {
                max_grad = grad[row * width + col].x;
            }
        }
    }
    return max_grad;
}

__device__ inline bool check_ngh(grad_cuda *grad, int height, int width, int row, int col)
{

    // iterate through pixel neighborhood

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (((row + i) >= 0) && (row + i) < height)
            {
                if (((col + j) >= 0) && (col + j) < width)
                {
                    if ((int) (grad[(row + i) * width + (col + j)].z) == strong)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

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
    float blur_kernel[3][3] = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}};

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

__global__ void sobel_kernel(unsigned char *gray, grad_cuda *grads, unsigned char *gray_out, int height, int width)

{
    float sobel_kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float sobel_kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    if (row < 1 || col < 1 || row >= (height - 1) || col >= (width - 1))
    {
        // Pixels at the image borders are left unchanged
        gray_out[row * width + col] = gray[row * width + col];
        grads[row * width + col].x = static_cast<float>(gray[row * width + col]);
        return;
    }

    double dX = 0, dY = 0;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            dX += sobel_kernelX[i + 1][j + 1] * gray[(row + i) * width + (col + j)]; // for gradient along x
            dY += sobel_kernelY[i + 1][j + 1] * gray[(row + i) * width + (col + j)]; // for gradient along y
        }
    }

    // Calculate magnitude and direction
    double mag = magnitude(dX, dY);
    double dir = direction(dX, dY);
    
    // Store gradient information in grads array
    grads[row * width + col].x = mag;
    grads[row * width + col].y = dir;
    gray_out[row * width + col] = (unsigned char)mag;
}

__global__ void nms_kernel(unsigned char *gray, unsigned char *gray_out,
                           grad_cuda *grads, grad_cuda *grads_out,
                           int height, int width)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    int linear_idx = row * width + col;

    if (row < 1 || col < 1 || row >= (height - 1) || col >= (width - 1))
    {
        // Pixels at the image borders are left unchanged
        grads_out[linear_idx] = grads[linear_idx];
        gray_out[linear_idx] = gray[linear_idx];
        return;
    }

    double mag = grads[linear_idx].x;
    double dir = grads[linear_idx].y;
    double pi_by_8 = 0.785398163397448309616 / 2;

    // Determine adjacent pixels based on gradient direction
    double pixel1_mag, pixel2_mag;
    if ((dir > (-1 * pi_by_8) && dir <= pi_by_8) || (dir > (7 * pi_by_8) && dir <= (-7 * pi_by_8))) 
    {
        pixel1_mag = grads[row * width + col - 1].x;
        pixel2_mag = grads[row * width + col + 1].x;
    }
    else if ((dir > pi_by_8 && dir <= (3 * pi_by_8)) || (dir > (-7 * pi_by_8) && dir <= (-5 * pi_by_8)))
    {
        pixel1_mag = grads[(row - 1) * width + col + 1].x;
        pixel2_mag = grads[(row + 1) * width + col - 1].x;
    }
    else if ((dir > (3 * pi_by_8) && dir <= (5 * pi_by_8)) || (dir > (-5 * pi_by_8) && dir <= (-3 * pi_by_8)))
    {
        pixel1_mag = grads[(row - 1) * width + col].x;
        pixel2_mag = grads[(row + 1) * width + col].x;
    }
    else
    {
        pixel1_mag = grads[(row - 1) * width + col - 1].x;
        pixel2_mag = grads[(row + 1) * width + col + 1].x;
    }


    // Suppress non-maximum pixels
    if (mag <= pixel1_mag || mag <= pixel2_mag)
    {
        grads_out[linear_idx].x = 0.0;
        gray_out[linear_idx] = 0;
    }
}

__global__ void threshholding_kernel(unsigned char *gray, unsigned char *gray_out,
                                    grad_cuda *grads, grad_cuda *grads_out, double max_grad,
                                    int height, int width)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    const double high_threshold_ratio = 0.1;
    const double low_threshold_ratio = 0.05;

    double high_thres = max_grad * high_threshold_ratio;
    double low_thres = max_grad * low_threshold_ratio;

    int linear_idx = row * width + col;

    if (grads[linear_idx].x >= high_thres)
    {
        grads_out[linear_idx].z = static_cast<float>(strong);
    }
    else if ((grads[linear_idx].x >= low_thres) && (grads[linear_idx].x < high_thres))
    {
        grads_out[linear_idx].z = static_cast<float>(weak);
    }
    else
    {
        grads_out[linear_idx].z = static_cast<float>(no_edge);
        grads_out[linear_idx].x = 0;
        gray_out[linear_idx] = 0;
    }
}

__global__ void edge_tracking_kernel(unsigned char *gray, grad_cuda *grads, unsigned char *gray_out, int height, int width)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row >= height || col >= width)
    {
        return;
    }

    int linear_idx = row * width + col;

    if ((int)(grads[linear_idx].z) == weak)
    {
        if (!check_ngh(grads, height, width, row, col))
        {
            // grads[linear_idx].x = 0;
            gray[linear_idx] = 0;
        }
    }
}

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

    // Allocate memory for image gradients on GPU
    grad_cuda *device_grads;
    cudaMalloc((void **)&device_grads, host_img.width * host_img.height * sizeof(grad_cuda));

    // Allocate memory for image gradient ouputs on GPU
    grad_cuda *device_grads_out;
    cudaMalloc((void **)&device_grads_out, host_img.width * host_img.height * sizeof(grad_cuda));

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

    // 2. Gaussian Blur
    start = clock();
    gaussianblur_kernel<<<gridDim, blockDim>>>(device_gray, device_gray_out, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.gaussian_blur = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.gaussian_blur;


    // 3. Determine intensity gradient
    start = clock();
    sobel_kernel<<<gridDim, blockDim>>>(device_gray_out, device_grads, device_gray, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.sobel = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.sobel;

    
    /* grad_cuda *temp;
    temp = (grad_cuda *)malloc(host_img.width * host_img.height * sizeof(grad_cuda));
    cudaMemcpy(temp, device_grads, host_img.width * host_img.height * sizeof(grad_cuda), cudaMemcpyDeviceToHost); */

    // 4. Non Maximum seperation
    start = clock();
    nms_kernel<<<gridDim, blockDim>>>(device_gray, device_gray_out, device_grads, device_grads_out, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.nms = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.nms;

    // Find max grad
    grad_cuda *host_grads;
    host_grads = (grad_cuda *)malloc(host_img.width * host_img.height * sizeof(grad_cuda));
    cudaMemcpy(host_grads, device_grads_out, host_img.width * host_img.height * sizeof(grad_cuda), cudaMemcpyDeviceToHost);

    double max_grad = find_max_grad(host_grads, host_img.height, host_img.width);

    // 5. Double Thresholding
    start = clock();
    threshholding_kernel<<<gridDim, blockDim>>>(device_gray_out, device_gray,
                                                device_grads_out, device_grads,
                                                max_grad,
                                                host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.double_thres = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.double_thres;

    // 6. Edge Tracking
    start = clock();
    edge_tracking_kernel<<<gridDim, blockDim>>>(device_gray, device_grads,
                                                device_gray_out, host_img.height, host_img.width);
    cudaDeviceSynchronize();
    end = clock();
    time.edge_track = ((double)end - (double)start) / CLOCKS_PER_SEC;
    time.total += time.edge_track;


    // Copy image data from device to host
    unsigned char *host_gray = (unsigned char *)malloc(host_img.width * host_img.height * sizeof(unsigned char));
    cudaMemcpy(host_gray, device_gray_out, host_img.width * host_img.height * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Unflatten pixel array
    unflatten_pixel_array(host_img, host_gray);

    // Write image
    write_PPM(host_img, write_file);

    // Free device memory space
    cudaFree(device_pixels);
    cudaFree(device_gray);
    cudaFree(device_gray_out);
    cudaFree(device_grads);
    cudaFree(device_grads_out);

    // Free host memory
    free(host_flat_pixels);
    free(host_gray);
    free(host_grads);
}
