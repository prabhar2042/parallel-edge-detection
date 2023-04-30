#include <iostream>
#include <getopt.h>

#include <string.h>
#include <unistd.h>
#include "canny.h"

#define MAX_FILE_NAME 200

void usage(const char *progname)
{
    printf("Usage: %s [options]\n", progname);
    printf("Program Options:\n");
    printf("  -r  --readFile <STRING>  Image read location\n");
    printf("  -w  --writeFile <STRING>  Image write location\n");
    printf("  -?  --help             This message\n");
}

int main(int argc, char *argv[])

{

    char read_file[MAX_FILE_NAME];
    char write_file[MAX_FILE_NAME];

    int option = 0;

    while ((option = getopt(argc, argv, "r:w:")) != -1)
    {
        switch (option)
        {
        case 'r':
            strncpy(read_file, optarg, MAX_FILE_NAME);
            break;
        case 'w':
            strncpy(write_file, optarg, MAX_FILE_NAME);
            break;
        case '?':
        default:
            usage(argv[0]);
            return 1;
        }
    }

    exec_time time;
    canny(time, read_file, write_file);
    print_time(time);
    // print_time_debug(time);

    return 0;
}
