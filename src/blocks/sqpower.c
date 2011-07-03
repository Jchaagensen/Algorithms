#ifndef __x86_64__
#   define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include "sq_dsp.h"
#include "sq_utils.h"

char* usage_text[] = 
{
    "                                                                   ",
    "NAME                                                               ",
    "   sqpower - computes the instantaneous power                      ",
    "SYNOPSIS                                                           ",
    "   sqpower [OPTIONS] ...                                             ",
    "DESCRIPTION                                                        ",
    "   -l number of samples to read in one go.                         "
};

unsigned int smpls_len = 100000;

int main(int argc, char **argv)
{

    int opt;

    while ((opt = getopt(argc, argv, "hl:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &smpls_len);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }

    int status = sq_power(stdin, stdout, smpls_len);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}