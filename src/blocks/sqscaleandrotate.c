#ifndef __x86_64__
    #define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sq_dsp.h>
#include <sq_utils.h>

char* usage_text[] = 
{
    "                                                                   ",
    "NAME                                                               ",
    "   sqscaleandrotate - multiplies the signal by a real value R and  ",
    "                      increments it's phase by a real value Theta  ",
    "                      (in radians)                                 ",
    "SYNOPSIS                                                           ",
    "   sqscaleandrotate [OPTIONS] ...                                  ",
    "DESCRIPTION                                                        ",
    "   -l number of samples to read in one go.                         ",
    "   -r Scaling factor                                               ",
    "   -t phase offset                                                 "
};


unsigned int data_len = 1000000;
float R = 1;
float theta = 0;

int main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hr:t:l:")) != -1)
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
                sscanf(optarg, "%f", &R);
                break;
            case 't':
                sscanf(optarg, "%f", &theta);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }
    
    int status = sq_scaleandrotate(stdin, stdout, data_len, R, theta);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
