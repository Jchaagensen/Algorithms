#ifndef __x86_64__
#   define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include "sq_constants.h"
#include "sq_dsp.h"
#include "sq_utils.h"

unsigned int smpls_len = 100000;

int main(int argc, char **argv)
{

    int opt;

    while ((opt = getopt(argc, argv, "l:")) != -1)
    {
        switch (opt)
        {
            case 'l':
                sscanf(optarg, "%u", &smpls_len);
                break;
        }
    }

    FILE* signal1 = fopen(argv[3], "r");
    FILE* signal2 = fopen(argv[4], "r");
    
    if(signal1 == NULL || signal2 == NULL)
    {
        sq_error_handle(ERR_STREAM_READ);
        exit(EXIT_FAILURE);
    }

    int status = sq_crossmultiply(signal1, signal2, stdout, smpls_len);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
