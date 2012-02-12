/*******************************************************************************

  File:    sq_windows.c
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

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "sq_windows.h"
#include "sq_constants.h"

void sq_make_window (float* window_buffer, unsigned int length, float (*window_function)(unsigned int, unsigned int))
{
    unsigned int n;
    for (n = 0; n < length; n++)
        window_buffer[n] = window_function(n, length);
}

int sq_make_window_from_name ( float* window_buffer, unsigned int length, char* window_name )
{
    float (*window_function)(unsigned int, unsigned int);
        
    if(!strcmp(window_name, "hann"))
        window_function = sq_window_hann;
    else if(!strcmp(window_name, "hamming"))
        window_function = sq_window_hamming;
    else if(!strcmp(window_name, "sine") || !strcmp(window_name, "cosine"))
        window_function = sq_window_cosine;
    else if(!strcmp(window_name, "lanczos"))
        window_function = sq_window_lanczos;
    else if(!strcmp(window_name, "triangular"))
        window_function = sq_window_triangular;
    else if(!strcmp(window_name, "gaussian"))
        window_function = sq_window_gaussian;
    else if(!strcmp(window_name, "bothalf"))
        window_function = sq_window_bothalf;
    else if(!strcmp(window_name, "tophalf"))
        window_function = sq_window_tophalf;
    else
        return ERR_UNKNOWN_WINDOW;
    
    sq_make_window(window_buffer, length, window_function);
    return 0;
}

float sq_window_hann (unsigned int n, unsigned int length)
{
    return 0.5 * (1 - cos( (double)(2.0*M_PI*n) / (double)(length-1) ));
}

float sq_window_hamming ( unsigned int n, unsigned int length )
{
    return 0.54 - 0.46 * cos( (double)(2.0*M_PI*n) / (double)(length-1) );
}

float sq_window_cosine ( unsigned int n, unsigned int length )
{
    return sin( (double)(M_PI*n)/(double)(length-1) );
}

float sq_window_lanczos(unsigned int n, unsigned int length)
{
    return SINC( ((double)2*n/(double)(length-1)) - 1 );
}

float sq_window_bothalf ( unsigned int n, unsigned int length )
{
    // kill one sample at beginning and end of each band
    if (n == 0) return 0;
    if (n == length/2-1) return 0;
    if (n == length/2) return 0;
    if (n == length-1) return 0;
    if (n <  length/2) return 1;
    if (n >= length/2) return 0;
}

float sq_window_tophalf ( unsigned int n, unsigned int length )
{
    // kill one sample at beginning and end of each band
    if (n == 0) return 0;
    if (n == length/2-1) return 0;
    if (n == length/2) return 0;
    if (n == length-1) return 0;
    if (n <= length/2) return 0;
    if (n >  length/2) return 1;
}

float sq_window_triangular ( unsigned int n, unsigned int length )
{
    return ( 2.0/(double)(length) ) * ((double)length/2.0  - fabs((double)n - (double)(length-1)/2.0 ));
}

float sq_window_gaussian ( unsigned int n, unsigned int length )
{
    float sigma = 0.4;
    return ( exp(   -0.5 * pow( (double)(n - 0.5*(length-1)) / (double)(sigma*0.5*(length-1) )  ,2)   )  );
}
