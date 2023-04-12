#include <iostream>
#include "ppm.h"

int main(int argc, char *argv[])
{
    printf("start \n");

    char read_file[] = "dataset/sample_640×426.ppm";
    char writ_file[] = "dataset/test.ppm";

    Image img;

    read_PPM(img, read_file);

    rgb_to_gray(img);

    write_PPM(img, writ_file);

    printf("done \n");
}
