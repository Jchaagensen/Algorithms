#ifndef __x86_64__
    #define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "sq_constants.h"
#include "sq_dsp.h"
#include "sq_utils.h"

char* usage_text[] = 
{
    "                                                                   ",
    "NAME                                                               ",
    "   sqmix - Centers the chosen frequency of the signal to the center",
    "           of the spectrum.                                        ",
    "SYNOPSIS                                                           ",
    "   sqmix [OPTIONS] ...                                             ",
    "DESCRIPTION                                                        ",
    "   -l number of samples to read in one go.                         ",
    "   -r radians per sample                                           ",
    "   -c frequency to heterodyne                                      "
};

unsigned int data_len = 1000000;
float channel = 0.0;
float rad_per_sample = 0.0;
const float TWO_PI = 2.0 * M_PI;

int main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hl:r:c:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &data_len);
                break;
            case 'r':
                sscanf(optarg, "%f", &rad_per_sample);
                break;
            case 'c':
                sscanf(optarg, "%f", &channel);
                rad_per_sample = TWO_PI * (channel / (float) SQ_STAGE1_FFT_LEN);  
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }
    
    int status = sq_mix(stdin, stdout, data_len, rad_per_sample);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
