/*******************************************************************************

  File:    window.c
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
#include <sq_windows.h>

char* usage_text[] = 
{
    "                                                                   ",
    "NAME                                                                   ",
    "   window - writes windows defined in the sq_windows module to stdout ", 
    "SYNOPSIS                                                           ",
    "   qwindow [OPTIONS] ...                                       ",
    "DESCRIPTION                                                        ",
    "   -l length of window                               ",
    "   -w name of window:                                              ",
    "      hann",
    "      hamming",
    "      sine/cosine"
};


unsigned int wndw_len = 1024;
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
    
    float* wnd = calloc(wndw_len, sizeof(float));
    
    unsigned int i;

    int status = sq_make_window_from_name(wnd, wndw_len, window_name);

    if(status < 0)
    {
        fprintf(stderr, "%s encountered a fatal error.", argv[0]);
        sq_error_handle(status);
        exit(EXIT_FAILURE);
    }
    
    sq_write_array(stdout, wnd, wndw_len, 1);
    
    exit(EXIT_SUCCESS);
}


