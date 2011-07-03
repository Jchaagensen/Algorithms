#ifndef __x86_64__
    # define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>

#include "sq_signals.h"
#include "sq_utils.h"

char* usage_text[] = 
{
    "                                                                       ",
    "NAME                                                                   ",
    "   sqgensine - generates a gaussian-noisy complex sinusoid of the form ",
    "               sin*SNR + noise, cos*SNR + noise, sin*SNR + noise, ...  ",
    "SYNOPSIS                                                               ",
    "   sqgensine [OPTIONS] ...                                             ",
    "DESCRIPTION                                                            ",
    "   -l Number of samples to do in one go                                ",
    "   -s Length of the sinusoid in terms of number of cycles              ",
    "   -a Length of a cycle in terms of number of samples                  ",
    "   -w Wavelength of the sinusoid                                       ",
    "   -n The SNR (Signal to Noise Ratio)                                  "
};


unsigned int nsamples = 1000000;
unsigned int length = 16777216;
unsigned int sine_array_length = 128;
float wavelength = 2.4f;
float SNR = 0.01;

int main(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "hl:s:a:w:n:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
            case 'l':
                sscanf(optarg, "%u", &nsamples);
                break;
            case 's':
                sscanf(optarg, "%u", &length);
                break;
            case 'a':
                sscanf(optarg, "%u", &sine_array_length);
                break;
            case 'w':
                sscanf(optarg, "%f", &wavelength);
                break;
            case 'n':
                sscanf(optarg, "%f", &SNR);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
        }
    }
    
    int status = sq_gen_sine(stdout, nsamples, length, sine_array_length, wavelength, SNR);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
