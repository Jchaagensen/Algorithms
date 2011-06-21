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
    "  -p  flag, scale appropriate for power values which have an exponential",
    "      distribution in the right tail.  (m - s) scaled to 0.  (m + 3s)   ",
    "      scaled to 255. Values outside of range are clipped.               ",
    "  -x  flag, power values not scaled                                     ",
    "  -d  fraction of columns to be discarded from each edge. This will make",
    "      the number of columns in the resulting image less than specified. ",
    "                                                                        "
};

float *imgbfr, *imgbfr_chopped;
unsigned int rows = 0;
unsigned int cols = 0;
float width_chop_fraction = 0.0;
float height_chop_fraction = 0.0;

int (*scale_fnctn)(float* img_buf, int rows, int cols);

int main(int argc, char *argv[])
{
    int opt;
    
    scale_fnctn = sq_linear_scale;
    
    while ((opt = getopt(argc, argv, "r:c:w:h:")) != -1)
    {
        switch (opt)
        {
            case 'r':
                sscanf(optarg, "%u", &rows);
                break;
            case 'c':
                sscanf(optarg, "%u", &cols);
                break;
            case 'w':
                sscanf(optarg, "%f", &width_chop_fraction);
                break;
            case 'h':
                sscanf(optarg, "%f", &height_chop_fraction);
                break;
        }
    }
    
    unsigned int col_first = cols * width_chop_fraction;
    unsigned int col_last = cols * (1 - width_chop_fraction);
    unsigned int col_count = col_last - col_first;
    
    unsigned int row_first = rows * height_chop_fraction;
    unsigned int row_last = rows * (1 - height_chop_fraction);
    unsigned int row_count = row_last - row_first;
    
    imgbfr = malloc(sizeof(float) * rows * cols);
    if(imgbfr == NULL)
    {
        sq_error_handle(ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    int read_status = sq_read_img(stdin, imgbfr, rows, cols);
    if(read_status < 0)
    {
        free(imgbfr);
        sq_error_handle(read_status);
        exit(EXIT_FAILURE);
    }
    
    imgbfr_chopped = malloc(sizeof(float) * row_count * col_count);
    if(imgbfr_chopped == NULL)
    {
        sq_error_handle(ERR_MALLOC);
        exit(EXIT_FAILURE);
    }
    
    int chop_status = sq_chop(imgbfr, imgbfr_chopped, rows, cols, width_chop_fraction, 0);
    if(chop_status < 0)
    {
        free(imgbfr);
        sq_error_handle(chop_status);
        exit(EXIT_FAILURE);
    }
    
    fprintf(stderr, "Image chopped by %s - number of columns: %d, number of rows: %d\n", argv[0], col_count, row_count);
    
    sq_write_img(stdout, imgbfr_chopped, row_count, col_count);
    free(imgbfr);
    free(imgbfr_chopped);
    
    exit(EXIT_SUCCESS);
}
