#ifndef SQ_WINDOWS_H
#define SQ_WINDOWS_H

void sq_make_window(float* window_buffer, unsigned int length, float (*window_function)(unsigned int, unsigned int));
int sq_make_window_from_name( float* window_buffer, unsigned int length, char* window_name);

float sq_window_hann(unsigned int n, unsigned int length);
float sq_window_hamming(unsigned int n, unsigned int length);

#endif