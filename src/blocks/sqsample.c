#ifndef __x86_64__
#define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include <sq_constants.h>
#include <sq_dsp.h>
#include <sq_utils.h>

unsigned int samples_len = SMPLS_PER_READ;

char* usage_text[] = 
{
    "NAME                                                                         ",
    "   sqsample - Takes 2-channel (quadrature) 8-bit data from the input stream, ",
    "               and writes samples in the form of floats to the output stream,",
    "               in chunks of a given number of samples.                       ",
    "SYNOPSIS                                                                     ",
    "   sqsample [OPTIONS] ...                                                    ",
    "   -l number of samples to read in one go.                                   "
};

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
                sscanf(optarg, "%u", &samples_len);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
        }
    }
    
    int status = sq_sample(stdin, stdout, samples_len);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
