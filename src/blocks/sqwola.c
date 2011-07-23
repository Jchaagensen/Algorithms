/*******************************************************************************

  File:    sqwola.c
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
#define _FILE_OFFSET_BITS 64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#include <sq_dsp.h>
#include <sq_utils.h>

//          1         2         3         4         5         6         7
// 123456789012345678901234567890123456789012345678901234567890123456789012
char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqwola - performs weight-overlap-add window on data in front of       ",
    "           FFT (sqfftw) to improve channel response characteristics     ",
    "SYNOPSIS                                                                ",
    "  sqwola [OPTIONS]                                                      ",
    "DESCRIPTION                                                             ",
    "  -l  pos. integer (required), samples length of subsequent FFT         ",
    "  -f  pos. odd integer (required), number of folds                      ",
    "  -o  pos. integer (required), overlap percentage: 0, 25, or 50         ",
    "  -w  flag, dump window coefficients to stdout and then exit            ",
    "                                                                        "
};

unsigned int fftlen;
unsigned int folds;
unsigned int overlap;
unsigned char is_window_dump = 0;

int main(int argc, char *argv[])
{
    unsigned int wndwi, readi, smpli, ffti;

    unsigned int readlen;

    unsigned char argflags = 0;
    int opt;

    while ((opt = getopt(argc, argv, "hl:f:o:w")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &fftlen);
                argflags |= 0x01;
                break;
            case 'f':
                sscanf(optarg, "%u", &folds);
                argflags |= 0x02;
                break;
            case 'o':
                sscanf(optarg, "%u", &overlap);
                argflags |= 0x04;
                break;
            case 'w':
                is_window_dump = 1;
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }
    if (!(argflags == 0x07))
    {
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }

    int status = sq_wola(stdin, stdout, fftlen, folds, overlap, is_window_dump);

    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


