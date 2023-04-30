
#ifndef __FILTER_H__
#define __FITLER_H__

/**
 * @brief kernel for gaussian blur
 *
 */
const float blur_kernel[3][3] = {{0.0625, 0.125, 0.0625},
                                 {0.125, 0.25, 0.125},
                                 {0.0625, 0.125, 0.0625}};

#endif