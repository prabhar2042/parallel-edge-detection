
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
    for (int i = 0; i < img.height; i++)
    {
        for (int j = 0; j < img.width; j++)
        {
            if (img.grads[i][j].thres_bin == weak)
            {
                if (!check_ngh(img, i, j))
                {
                    img.grads[i][j].mag = 0;
                    img.pixels[i][j].gray.value = 0;
                }
            }
        }
    }
}
