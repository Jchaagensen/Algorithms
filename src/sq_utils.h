#ifndef SQ_UTILS_H
#define SQ_UTILS_H

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <sys/types.h>
#include <fftw3.h>

/** 
 * Prints a processing block's usage to the output stream.
 * @param usage_text A two-dimensional character array; each row is printed on a new line.
 */
void print_usage(char** usage_text);

/** Takes a string and writes it as a log message to a given stream.
 * @param message The log message.
 * @param outstream The stream to write to, eg. stderr.
 */
void write_log(char* message, FILE* outstream);

void alloc_char_2d(signed char** array, uint nrows, unsigned int ncolumns);
void alloc_float_2d(float** array, uint nrows, unsigned int ncolumns);

/**
 * Reads a sequence of float values from a float array, and writes them as text values to the output stream, in the specified number of columns.
 * @param inarray Input array of floats.
 * @param outstream Stream to write the columns to.
 * @param ncolumns Number of columns the samples are to be shown in.
 * @return If successful, the number of samples processed. Else, a negative error code.
 */
int sq_read_array(float inarray[], FILE* outstream, int ncolumns);

/**
 * Reads a sequence of float values from an input stream, and writes them as text values to the output stream, in the specified number of columns.
 * @param instream Input stream of floats.
 * @param outstream Stream to write the columns to.
 * @param ncolumns Number of columns the samples are to be shown in.
 * @return If successful, the number of samples processed. Else, a negative error code.
 */
int sq_read_stream(FILE* instream, FILE* outstream, int ncolumns);

/**
 * Takes 2-channel (quadrature) 8-bit data from the input stream,
 * and writes samples in the form of floats to the output stream,
 * in chunks of a given number of samples.
 * @param instream Input stream with 2-channel 8-bit data.
 * @param outstream Output stream of floats
 * @param nsamples Number of samples to process at a time
 * @return Code; negative if error.
 */
int sq_sample( FILE* instream, FILE* outstream, unsigned int nsamples, uint64_t filesize);

/**
 * Swaps left and right halves of an array of complex numbers
 * @param buffer A pointer to an fftwf_complex array
 * @param length Length of input array
 */
void sq_channelswap(fftwf_complex* buffer, unsigned int length);

/**
 * Generates a random guassian number between 0 and 1
 * @return A float value between 0 and 1
 */
float sq_randgaus();

/**
 * Print an error message to stderr
 * @param message Message string
 */
void sq_error_print(char* message);

/**
 * Print the error message for the specified error code
 * @param errcode Error code
 */
void sq_error_handle(int errcode);

#endif
