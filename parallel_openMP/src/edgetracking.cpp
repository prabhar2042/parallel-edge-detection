
#include "edgetracking.h"

bool check_ngh(Image &img, int row, int col)
{

    // iterate through pixel neighborhood

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (((row + i) >= 0) && (row + i) < img.height)
            {
                if (((col + j) >= 0) && (col + j) < img.width)
                {
                    if (img.grads[row + i][col + j].thres_bin == strong)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void edge_tracking(Image &img)
{
    int tile_size = 64;

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < img.height - tile_size; i+= tile_size)
    {
        for (int j = 0; j < img.width - tile_size; j+= tile_size)
        {
            for (int ii = i; ii < tile_size; ii++) {
                for (int jj = j; jj < tile_size; jj++) {
                    if (img.grads[ii][jj].thres_bin == weak)
                    {
                        if (!check_ngh(img, ii, jj))
                        {
                            img.grads[ii][jj].mag = 0;
                            img.pixels[ii][jj].gray.value = 0;
                        }
                    }
                }
            }
            
        }
    }
}
