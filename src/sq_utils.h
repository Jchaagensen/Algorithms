#ifndef SQ_UTILS_H
#define SQ_UTILS_H

#include <stdio.h>

/** 
 * Prints a processing block's usage to the output stream.
 * @param usage_text A two-dimensional character array; each row is printed on a new line.
 */
void print_usage(char* usage_text[]);

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

#endif