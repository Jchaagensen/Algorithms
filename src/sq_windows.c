#include <math.h>
#include <string.h>

#include "sq_windows.h"
#include "sq_constants.h"

void sq_make_window (float* window_buffer, unsigned int length, float (*window_function)(unsigned int, unsigned int))
{
    unsigned int n;
    for (n = 0; n < length; n++)
        window_buffer[n] = sq_window_hann(n, length);
}

int sq_make_window_from_name ( float* window_buffer, unsigned int length, char* window_name )
{
    float (*window_function)(unsigned int, unsigned int);
    
    if(!strcmp(window_name, "hann"))
        window_function = sq_window_hann;
    else if(!strcmp(window_name, "hamming"))
        window_function = sq_window_hamming;
    else
        return ERR_UNKNOWN_WINDOW;
    
    sq_make_window(window_buffer, length, window_function);
    return 0;
}

float sq_window_hann (unsigned int n, unsigned int length)
{
    return 0.5 * (1 - cos( (2.0*M_PI*n)/(length-1) ));
}

float sq_window_hamming ( unsigned int n, unsigned int length )
{
    return 0.54 - 0.46 * cos( (2.0*M_PI*n)/(length-1) );
}

