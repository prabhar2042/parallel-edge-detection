#include "blur.h"

void gaussian_Blur(Image &img)
{

    // Allocate a new image to store the blurred image
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.height, std::vector<Pixel>(img.width));

    // Apply the kernel to each pixel of the image
    for (int row = 1; row < img.height - 1; ++row)
    {
        for (int col = 1; col < img.width - 1; ++col)
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
            blurredImg.pixels[row][col].gray.value = static_cast<unsigned char>(sum);
        }
    }

    // Copy the blurred image back to the original image
    img = blurredImg;
}
