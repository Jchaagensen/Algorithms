/*******************************************************************************

  File:    sqimag.c
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

#include <sq_dsp.h>
#include <sq_utils.h>

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