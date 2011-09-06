/*******************************************************************************

  File:    sqcrossmultiply.c
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
#   define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include <sq_constants.h>
#include <sq_dsp.h>
#include <sq_utils.h>

char* usage_text[] = 
{
    "                                                                               ",
    "NAME                                                                           ",
    "   sqcrossmultiply - multiplies samples of one signal to the corresponding     ",
    "                     samples of the other.                                     ",
    "                     If the two signals are a1, a2, a3, ... and b1, b2, b3, ...",
    "                     the result would be a1*b1, a2*b2, a3*b3, ...              ",
    "SYNOPSIS                                                                       ",
    "   sqcrossmultiply [OPTIONS] file1 file2                                       ",
    "DESCRIPTION                                                                    ",
    "   -l number of samples to read in one go.                                     ",
    "                                                                               "
};
int arrlen = sizeof(usage_text)/sizeof(*usage_text);

unsigned int smpls_len = 100000;

int main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hl:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text, arrlen);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &smpls_len);
                break;
            default:
                print_usage(usage_text, arrlen);
                exit(EXIT_FAILURE);
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
