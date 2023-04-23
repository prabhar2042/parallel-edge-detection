#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include "ppm.h"
#include "image.h"

/**
 * @brief reads rgb image
 *
 * @param img
 * @param filename
 * @return true
 * @return false
 */
bool read_PPM(Image &img, const char *filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open image file." << std::endl;
        return FAIL;
    }

    // Read PPM header
    std::string magic;
    int maxval;
    file >> magic >> img.width >> img.height >> maxval;
    if (magic != "P3" && magic != "P6")
    {
        std::cerr << "Invalid PPM format." << std::endl;
        return FAIL;
    }

    // Allocate memory for image data
    img.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // Read image data
    if (magic == "P3")
    {
        // P3 format: ASCII
        int r, g, b;
        for (int row = 0; row < img.height; ++row)
        {
            for (int col = 0; col < img.width; ++col)
            {
                file >> r >> g >> b;
                img.pixels[row][col].rgb.r = static_cast<unsigned char>(r);
                img.pixels[row][col].rgb.g = static_cast<unsigned char>(g);
                img.pixels[row][col].rgb.b = static_cast<unsigned char>(b);
            }
        }
    }
    else
    {
        // P6 format: binary
        if (maxval != 255)
        {
            std::cerr << "Unsupported maxval value." << std::endl;
            return FAIL;
        }
        file.get(); // Read and discard newline character after header
        for (int row = 0; row < img.height; ++row)
        {
            file.read(reinterpret_cast<char *>(&img.pixels[row][0]), img.width * sizeof(Pixel));
        }
    }
    file.close();

    return PASS;
}

/**
 * @brief writes gray scale image
 *
 * @param filename
 * @param img
 */
void write_PPM(const Image &img, const char *filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to create image file." << std::endl;
        return;
    }

    // Write PPM header
    file << "P5\n"
         << img.width << " " << img.height << "\n255\n";

    // Write image data
    for (int row = 0; row < img.height; ++row)
    {
        for (int col = 0; col < img.width; ++col)
        {
            const Pixel &pixel = img.pixels[row][col];
            unsigned char value = pixel.gray.value;
            file.write(reinterpret_cast<const char *>(&value), sizeof(unsigned char));
        }
    }
    file.close();
}


__global__ void rgb_to_gray_kernel(Image *img) {

}

__global__ void blur_kernel(Image *img) {

}

__global__ void sobel_kernel(Image *img) {

}

__global__ void nms_kernel(Image *img) {

}

__global__ void threshholding_kernel(Image *img) {

}

__global__ void edge_tracking_kernel(Image *img) {

}

void canny(char *read_file, char *write_file) {

    clock_t start, end;
    time.total = 0;
    Image img;

    // read image
    read_PPM(img, read_file);

    const int threadsPerBlock = 512;
    const int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;

    rgb_to_gray_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);

    blur_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);
    
    sobel_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);

    nms_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);

    threshholding_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);

    edge_tracking_kernel<<<blocks, threadsPerBlock>>>(device_img, device_result);

}
