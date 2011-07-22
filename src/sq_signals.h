#ifndef SQ_SIGNALS_H
#define SQ_SIGNALS_H

#include <stdio.h>

/**
 * Generate a sinewave with gaussian noise 
 * @param outstream output stream
 * @param nsamples Number of samples to read/process/write in one go
 * @param length Length of sinewave
 * @param sin_arr_length Number of samples in one period
 * @param wavelength Wavelength of sinewave
 * @param SNR The signal-to-noise ratio
 */
int sq_gen_sine(FILE* outstream,
                unsigned int nsamples,
                unsigned int length,
                unsigned int sin_arr_length,
                float wavelength,
                float SNR );

#endif
