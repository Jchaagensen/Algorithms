#ifndef __x86_64__
    #define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sq_utils.h"
#include "sq_imaging.h"
#include "sq_constants.h"

//          1         2         3         4         5         6         7
// 123456789012345678901234567890123456789012345678901234567890123456789012
char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqpnm - reads series of float values in raster-scan order (top-left to",
    "          bottom-right), scales the values, and then outputs the values ",
    "          in 8-bit grayscale (PGM) image format.  Default scale         ",
    "          algorithm is linear.  Smallest value scaled to 0.  Largest    ",
    "          value scaled to 255.                                          ",
    "SYNOPSIS                                                                ",
    "  sqpnm [OPTIONS] ...                                                   ",
    "DESCRIPTION                                                             ",
    "  -r  pos. integer (required), image rows                               ",
    "  -c  pos. integer (required), image columns                            ",
    "  -a  Number of lines to average                                        ",
    "  -p  flag, scale appropriate for power values which have an exponential",
    "      distribution in the right tail.  (m - s) scaled to 0.  (m + 3s)   ",
    "      scaled to 255. Values outside of range are clipped.               ",
    "  -x  flag, power values not scaled                                     ",
    "                                                                        "
};

float *imgbfr;
float* imgbfr_avg;
unsigned int rows = 0;
unsigned int cols = 0;
unsigned int averagelines = 0;
float chop_fraction = 0.0;

int (*scale_fnctn)(float* img_buf, int rows, int cols);

int main(int argc, char *argv[])
{
    int opt;

    scale_fnctn = sq_linear_scale;

    while ((opt = getopt(argc, argv, "hr:c:a:px")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'r':
                sscanf(optarg, "%u", &rows);
                break;
            case 'c':
                sscanf(optarg, "%u", &cols);
                break;
            case 'a':
                sscanf(optarg, "%u", &averagelines);
                break;
            case 'p':
                scale_fnctn = sq_power_scale;
                break;
            case 'x':
                scale_fnctn = sq_no_scale;
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "Allocating image\n");
    
    imgbfr = malloc(sizeof(float) * rows * cols);

    fprintf(stderr, "Allocated image\n");
    if(imgbfr == NULL)
    {
        sq_error_handle(ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    fprintf(stderr, "reading image\n");
    int read_status = sq_read_img(stdin, imgbfr, rows, cols);
    
    fprintf(stderr, "read image\n");
    if(read_status < 0)
    {
        free(imgbfr);
        sq_error_handle(read_status);
        exit(EXIT_FAILURE);
    }
fprintf(stderr, "Scaling image\n");

    scale_fnctn(imgbfr, rows, cols);
    
    fprintf(stderr, "Scaled image\n");
    // Now optionally average the lines as desired
    if(averagelines > 0)
    {
        imgbfr_avg = malloc(sizeof(float) * rows/averagelines * cols);
        sq_average_lines(imgbfr, rows, cols, imgbfr_avg, averagelines);
        sq_write_pnm(stdout, imgbfr_avg, rows/averagelines, cols);
        scale_fnctn(imgbfr_avg, rows/averagelines, cols);
        free(imgbfr_avg);
    }
    else
    {
        sq_write_pnm(stdout, imgbfr, rows, cols);
        free(imgbfr);
    }

    fprintf(stderr, "Exiting image\n");
    exit(EXIT_SUCCESS);
}
