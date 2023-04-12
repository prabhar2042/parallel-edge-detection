#include "image.h"

void rgb_to_gray(Image &img)
{
   for (int row = 0; row < img.height; ++row)
   {
      for (int col = 0; col < img.width; ++col)
      {

         int grayValue = 0.299 * img.pixels[row][col].rgb.r + 0.587 * img.pixels[row][col].rgb.g + 0.114 * img.pixels[row][col].rgb.b;
         img.pixels[row][col].gray.value = grayValue;
      }
   }
}
