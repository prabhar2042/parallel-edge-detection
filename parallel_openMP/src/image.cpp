#include "image.h"
#include <omp.h>
#include <cstdio>

void rgb_to_gray(Image &img)
{


#pragma omp parallel for schedule(static)

   for (int row = 0; row < img.height; ++row)
   {

#pragma omp simd
      for (int col = 0; col < img.width; ++col)
      {

         float grayValue = 0.299 * img.pixels[row][col].rgb.r + 0.587 * img.pixels[row][col].rgb.g + 0.114 * img.pixels[row][col].rgb.b;
         img.pixels[row][col].gray.value = static_cast<unsigned char>(grayValue);
      }
}

}

void padd_image(Image &img, int padd_size)
{

   int numRows = img.height;
   int numCols = img.width;

   // Pad top and bottom
   for (int i = 0; i < padd_size; i++)
   {
      std::vector<Pixel> row(numCols, Pixel{0, 0, 0});
      img.pixels.insert(img.pixels.begin(), row);
      img.pixels.insert(img.pixels.end(), row);
   }

   // Pad left and right
   for (int i = 0; i < numRows + 2 * padd_size; i++)
   {
      for (int j = 0; j < padd_size; j++)
      {
         img.pixels[i].insert(img.pixels[i].begin(), Pixel{0, 0, 0});
         img.pixels[i].insert(img.pixels[i].end(), Pixel{0, 0, 0});
      }
   }

   // Update image dimensions
   img.width = numCols + 2 * padd_size;
   img.height = numRows + 2 * padd_size;
}

void debug_print(Image &img)
{
   printf("/**********************************/\n");
   printf("image dimensions [%d,%d]\n", img.height, img.width);
   printf("maximum gradient magnitude : %f\n", img.max_grad);
   for (int row = 0; row < img.height; ++row)
   {
      for (int col = 0; col < img.width; ++col)
      {
         {
            // printf("pixel [%d,%d], value : %d, magnitude: %f, direction: %f bin: %d\n", row, col, img.pixels[row][col].gray.value, img.grads[row][col].mag, img.grads[row][col].dir, img.grads[row][col].thres_bin);
         }
      }
   }

   printf("/**********************************/\n");
}
