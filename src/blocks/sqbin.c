#ifndef __x86_64__
#   define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sq_dsp.h>
#include <sq_utils.h>

//          1         2         3         4         5         6         7
// 123456789012345678901234567890123456789012345678901234567890123456789012
char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqbin -  bin samples to a smaller number of samples",
    "SYNOPSIS                                                                ",
    "  sqbin [OPTIONS] ...                                                  ",
    "DESCRIPTION                                                             ",
    "  -i  Input number of samples ",
    "  -o  Number of samples in the output for the given input number"
};

unsigned int in_length = 4096;
unsigned int out_length = 4096;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hi:o:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'i':
                sscanf(optarg, "%u", &in_length);
                break;
            case 'o':
                sscanf(optarg, "%u", &out_length);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }

    int status = sq_bin(stdin, stdout, in_length, out_length);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
