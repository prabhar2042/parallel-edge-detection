#ifndef __EDGETRACKING_H__
#define __EDGETRACKING_H__

#include "image.h"

bool check_ngh(Image &img, int row, int col);
void edge_tracking(Image &img);

#endif