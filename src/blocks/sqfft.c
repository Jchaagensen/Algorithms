/*******************************************************************************

  File:    sqfft.c
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

#include <fftw3.h>

#include <sq_dsp.h>
#include <sq_utils.h>

//          1         2         3         4         5         6         7
// 123456789012345678901234567890123456789012345678901234567890123456789012
char *usage_text[] =
{
    "                                                                        ",
    "NAME                                                                    ",
    "  sqfftw - computes forward FFT.  Input is time-series of float values: ",
    "           (real)(imag)(real)(imag)...  Output is float values          ",
    "           arranged: (most neg. real)(most neg. imag) ... (DC real)     ",
    "           (DC imag) ... (most pos. real)(most pos. imag)               ",
    "SYNOPSIS                                                                ",
    "  sqfftw [OPTIONS] ...                                                  ",
    "DESCRIPTION                                                             ",
    "  -l  integer length of transform                                       ",
    "  -c  conjugate input data (invert freq spectrum neg. <--> pos.)        ",
    "  -m  measure plan instead of estimate.  This will cause an initial     ",
    "      delay but should result in an optimized transform.                ",
    "  -i inverse transform                                                  ",
    "                                                                        "
};

unsigned char is_conjugated = 0;
unsigned char is_measured = 0;
unsigned int fft_len = 0;
unsigned char inverse = 0;

int main(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "hl:cmi")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
            break;
            case 'l':
                sscanf(optarg, "%u", &fft_len);
                break;
            case 'c':
                is_conjugated = 1;
                break;
            case 'm':
                is_measured = 1;
                break;
            case 'i':
                inverse = 1;
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_SUCCESS);
        }
    }

    int status = sq_fft(stdin, stdout, fft_len, is_conjugated, is_measured, inverse);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
