/*******************************************************************************

  File:    sqconjugate.c
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
    "   -l number of samples to read in one go.                         ",
    "                                                                   "
};
int arrlen = sizeof(usage_text)/sizeof(*usage_text);

unsigned int data_len = 1000000;

int main(int argc, char **argv)
{
    int opt;

    while ((opt = getopt(argc, argv, "hl:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text, arrlen);
                exit(EXIT_SUCCESS);
            case 'l':
                sscanf(optarg, "%u", &data_len);
                break;
            default:
                print_usage(usage_text, arrlen);
                exit(EXIT_SUCCESS);
        }
    }
    
    int status = sq_conjugate(stdin, stdout, data_len);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        print_usage(usage_text, arrlen);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
