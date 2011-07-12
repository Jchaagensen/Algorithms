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
    "   sqconjugate - computes the complex conjugate of complex samples.",
    "                 re, im, re, im, ... to re, -im, re, -im, ...      ",
    "SYNOPSIS                                                           ",
    "   sqconjugate [OPTIONS] ...                                       ",
    "DESCRIPTION                                                        ",
    "   -l number of samples to read in one go.                         "
};

unsigned int data_len = 1000000;

int main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hl:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
            case 'l':
                sscanf(optarg, "%u", &data_len);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
        }
    }
    
    int status = sq_conjugate(stdin, stdout, data_len);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
