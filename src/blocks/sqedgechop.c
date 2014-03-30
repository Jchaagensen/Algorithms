/*******************************************************************************

  File:    sqedgechop.c
  Project: SETIkit
  Authors: Aditya Bhatt <aditya at adityabhatt dot org>

  Copyright 2011 The SETI Institute

  SETIkit is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SETIkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with SETIkit.  If not, see <http://www.gnu.org/licenses/>.

  Implementers of this code are requested to include the caption
  "Licensed through SETI" with a link to setiQuest.org.

  For alternate licensing arrangements, please contact
  The SETI Institute at www.seti.org or setiquest.org. 

*******************************************************************************/

#ifndef __x86_64__
#define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sq_utils.h>
#include <sq_imaging.h>
#include <sq_constants.h>

char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqedgechop - reads series of float values in raster-scan order        ",
    "               (top-left to bottom-right), scales the values, and then  ",
    "               discards (chops off) a specified fraction of lines from  ",
    "               the top-and-bottom edges or the left-and-right edges,    ",
    "SYNOPSIS                                                                ",
    "  sqedgechop [OPTIONS] ...                                              ",
    "DESCRIPTION                                                             ",
    "  -r  pos. integer (required), image rows                               ",
    "  -c  pos. integer (required), image columns                            ",
    "  -x  fraction of columns to be discarded from the left and right edges.",
    "  -y  fraction of columns to be discarded from the top and bottom edges.",
    "                                                                        "
};
int arrlen = sizeof(usage_text)/sizeof(*usage_text);

float *imgbfr, *imgbfr_chopped;
unsigned int rows = 0;
unsigned int cols = 0;
float width_chop_fraction = 0.0;
float height_chop_fraction = 0.0;

int main(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "hr:c:x:y:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text, arrlen);
                exit(EXIT_SUCCESS);
            case 'r':
                sscanf(optarg, "%u", &rows);
                break;
            case 'c':
                sscanf(optarg, "%u", &cols);
                break;
            case 'x':
                sscanf(optarg, "%f", &width_chop_fraction);
                break;
            case 'y':
                sscanf(optarg, "%f", &height_chop_fraction);
                break;
            default:
                print_usage(usage_text, arrlen);
                exit(EXIT_SUCCESS);
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
    
    int chop_status = sq_imchop(imgbfr, imgbfr_chopped, rows, cols, width_chop_fraction, 0);
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
