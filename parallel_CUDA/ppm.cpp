#include "ppm.h"

/**
 * @brief reads rgb image
 */
void read_PPM(Image &img, const char *filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open image file." << std::endl;
    }

    // Read PPM header
    std::string magic;
    int maxval;
    file >> magic >> img.width >> img.height >> maxval;
    if (magic != "P3" && magic != "P6")
    {
        std::cerr << "Invalid PPM format." << std::endl;
    }

    // Allocate memory for image data
    img.pixels = (Pixel **)malloc(img.height * sizeof(Pixel *));
    for (int i = 0; i < img.height; i++)
    {
        img.pixels[i] = (Pixel *)malloc(img.width * sizeof(Pixel));
    }

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
        }
        file.get(); // Read and discard newline character after header
        for (int row = 0; row < img.height; ++row)
        {
            file.read(reinterpret_cast<char *>(&img.pixels[row][0]), img.width * sizeof(Pixel));
        }
    }
    file.close();

    
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