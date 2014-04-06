/*******************************************************************************

  File:    sq_dsp.h
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

#ifndef SQ_DSP_H
#define SQ_DSP_H

#include <stdio.h>

/**
 * Takes a stream of floats (alternating real, imaginary) as input signal
 * and writes the instantaneous power samples to the output stream.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 * @return Code; negative if error.
 */
int sq_power(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Windows the input signal. This function no longer does overlaps
 * which has been moved to the sq_overlap2x function.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param window_length Length of window
 * @param window_name one of the predefined window names
 */
int sq_window( FILE* instream, FILE* outstream, unsigned int wndw_len, char* window_name);

/**
 * Takes a stream of sample floats as input signal and returns the 
 * real or imaginary component of that signal.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 * @param component The REAL (0) or IMAGINARY (1) component
 */
int sq_component(FILE* instream, FILE* outstream, unsigned int in_length, int component);

/**
 * Takes a stream of sample floats as input signal and returns the 
 * real component of that signal.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 */
int sq_real(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Takes a stream of sample floats as input signal and returns the 
 * imaginary component of that signal.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 */
int sq_imag(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Computes the Fast Fourier Transform (FFT) of the sequence of floats in the
 * input stream and returns the sequence of floats of it's frequency domain
 * representation.
 * @param instream Input stream of float data in the time domain
 * @param outstream Output stream of float data in the frequency domain
 * @param fft_len The length of the FFT
 */
int sq_fft(FILE* instream, FILE* outstream, 
           unsigned int fft_len, 
           unsigned char is_inverted, 
           unsigned char is_measured,
           unsigned char inverse
          );

/**
 * Takes a signal and adds a complex DC offset to it.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length
 * @param real_delta The real part of the DC offset
 * @param imag_delta The imaginary part of the DC offset
 */
int sq_offset(FILE* instream, FILE* outstream, 
              unsigned int in_length, 
              float real_delta, 
              float imag_delta );

/**
 * Computes the average value of each raster and subtracts off the average from raster.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 */
int sq_subavg(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Computes the complex conjugate of a signal.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data which contains the conjugate samples
 * @param in_length Number of samples to process at a time
 */
int sq_conjugate(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Multiplies a signal by a constant R*exp(j*Theta), where R is the scaling factor
 * and Theta is the phase.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data which contains the conjugate samples
 * @param in_length Number of samples to process at a time
 * @param scale_factor The scaling factor R
 * @param radians The phase Theta
 */
int sq_scaleandrotate(FILE* instream, FILE* outstream, 
                      unsigned int in_length,
                      float scale_factor,
                      float radians );

/**
 * Scales the input signal by a given factor.
 * @param instream Input stream of float data
 * @param outstream Output stream of scaled float data
 * @param in_length Number of samples to process at a time
 * @param scale_factor The scaling factor R
 */
int sq_scale(FILE* instream, FILE* outstream, unsigned int in_length, float scale_factor);

/**
 * Adds a constant value to the instantaneous phase of each sample in the signal.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 * @param radians The value to be added to the instantaneous phase
 */
int sq_rotate(FILE* instream, FILE* outstream, unsigned int in_length, float radians);

/**
 * Heterodynes a chosen frequency of the signal to zero (center of the spectrum) -
 * this routine is useful for shifting the signal to the left/right by a tunable amount.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param radians Frequency to be centered
 */
int sq_mix(FILE* instream, FILE* outstream, unsigned int in_length, float radians);

/**
 * Weighted Overlap-Add window
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param fftlen Length of FFT to be used
 * @param folds Number of folds
 * @param overlap Window overlap %
 * @param is_window_dump If 1, writes the window itself to the output stream. If 0, proceeds to write the windowed data to the output stream.
 */
int sq_wola(FILE* instream, FILE* outstream,
            unsigned int fftlen,
            unsigned int folds,
            unsigned int overlap,
            unsigned char is_window_dump);

/**
 * Takes a stream of floats (alternating real, imaginary) as input signal
 * and writes the absolute value of the signal to the output stream.
 * These will be of the form real, 0, real, 0, ...
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 * @return Code; negative if error.
 */
int sq_abs(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Cross-multiplies two input signals' samples and writes the resulting samples to the output stream.
 * @param instream1 Input stream of float data of signal #1
 * @param instream2 Input stream of float data of signal #2
 * @param outstream Output stream of float data that is the product of signals #1 and #2
 * @param in_length Number of samples to process at a time
 * @return Code; negative if error.
 */
int sq_crossmultiply(FILE* instream1, FILE* instream2, FILE* outstream, unsigned int in_length);

/**
 * Multiplies the input data, one raster at a time, but a transfer function which is provided.
 * This is typically used to bandpass filter a set of data by applying this function in the
 * Fourier domain. The input transfer function file is an ascii file with one row of numbers.
 * The number of lines in the file must be equal to the number of in_length supplied on the
 * command line.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 * @param bpfile a text file containing the bandpass coefficients 
 * (these will _multiply_ against data, not divide, you decide where the cutoff will be)
 */
int sq_bandpass(FILE* instream, FILE* outstream, unsigned int in_length, char * bpfile);

/**
 * Performs a "fft flip" on the raster and outputs. The definition of an fft-flip is to
 * take the top half of the samples and swap them (without rearrangement) with the bottom
 * half of the samples. Useful to convert the output of an FFT to human-readable spectrum.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples to process at a time
 */
int sq_fftflip(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Bins the specified number of contiguous samples from the input stream into a new (smaller) specified number of samples
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one binning iteration
 * @param out_length Number of samples that are written out after one binning iteration
 */
int sq_bin(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length);

/**
 * Finds the peak value from a specified number n (bin-size) of contiguous samples from the input 
 * stream, outputting a new stream containing with N/n samples containing the max value in each
 * bin. Modeled after "peak hold" function of a spectrum analyzer.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one max hold (binning) iteration
 * @param out_length Number of samples that are written out after one max hold iteration
 */
int sq_maxhold(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length);

/**
 * Chops out samples from the left or right hand side of the specified number of 
 * contiguous samples from the input stream, delivering to a new (smaller) specified 
 * number of samples in the output stream. Notice that the number to be chopped off is
 * the difference between in_length and out_length. This is intentional to make scripting
 * easier to write.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one binning iteration
 * @param out_length Number of samples that are written out after one binning iteration
 * @param side Either 'r' or 'l' (case insensitive) indicating which side to chop.
 */
int sq_sidechop(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length, char side);

/** 
 * Sum N raster lines into one. This function reduces the number
 * of rasters produced by a factor of N. Note that this does a
 * sum and not an average.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length the usual in_length, length of one raster line
 * @param num_to_sum the N mentioned above, number of rasters to combine into one
 */
int sq_sum(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int num_to_sum);

/**
 * Discard samples on left and right of spectrum
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one chop iteration
 * @param chop_fraction Fraction of spectrum to discard on both edges of spectrum
 */
int sq_chop(FILE* instream, FILE* outstream, unsigned int in_length, float chop_fraction);

/**
 * Pad left and right of spectrum (raster) with zeros
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one chop iteration
 * @param out_length Number of samples to be written in one block, must be > in_length
 */
int sq_pad(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length);

/**
 * Convert the instream (binary floats) to an output stream in ascii format.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one iteration (actually makes no difF)
 */
int sq_ascii(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * Extract the phase-part of the data (divide each data point by its
 * absolute value. Output real (cosine) and imaginary (sine) parts
 * of phasor as floats.
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one iteration (actually makes no difF)
 */
int sq_phase(FILE* instream, FILE* outstream, unsigned int in_length);

/**
 * To be used before a (eg Hann) window function, this utility introduces 50% overlaps between
 * adjaent chunks of data. For input data length N, the output of this function will be 2N in 
 * length. Each output chunk is the concatenation of two reads length L/2 each 
 * (where L is -l input size option).
 * @param instream Input stream of float data
 * @param outstream Output stream of float data
 * @param in_length Number of samples given as input to one iteration (actually makes no difF)
 */
int sq_overlap2x(FILE* instream, FILE* outstream, unsigned int in_length);

#endif
