#ifndef __x86_64__
#   define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sq_dsp.h"
#include "sq_utils.h"

//          1         2         3         4         5         6         7
// 123456789012345678901234567890123456789012345678901234567890123456789012
char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqimag - inputs time-series of complex (real,imag) float coefficient  ",
    "           pairs.  Outputs only the imaginary coefficients and discards the  ",
    "           real coefficients: (r1,i1)(r2,i2)... --> i1, i2, ...    ",
    "SYNOPSIS                                                                ",
    "  sqreal [OPTIONS] ...                                                  ",
    "DESCRIPTION                                                             ",
    "  -l  number of samples to process in one go.                           "
};

unsigned int sblen = 0;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "l:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &sblen);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }

    int status = sq_imag(stdin, stdout, sblen);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}