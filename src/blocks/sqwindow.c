/*******************************************************************************

  File:    sqwindow.c
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

char* usage_text[] = 
{
    "                                                                   ",
    "NAME                                                               ",
    "   sqwindow - applies a hann window with 50% overlap               ",
    "SYNOPSIS                                                           ",
    "   sqwindow [OPTIONS] ...                                       ",
    "DESCRIPTION                                                        ",
    "   -l number of samples to read in one go.                         "
    "   -w name of window:                                              ",
    "      hann",
    "      hamming"
};


unsigned int wndw_len = 0;
char window_name[10];

int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "hl:w:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_usage(usage_text);
                exit(EXIT_FAILURE);
            case 'l':
                sscanf(optarg, "%u", &wndw_len);
                break;
            case 'w':
                sscanf(optarg, "%s", window_name);
                break;
            default:
                print_usage(usage_text);
                exit(EXIT_FAILURE);
        }
    }
    
    int status = sq_window(stdin, stdout, wndw_len, window_name);
    
    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


