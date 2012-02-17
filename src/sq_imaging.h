/*******************************************************************************

  File:    sq_imaging.h
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

#ifndef SQ_IMAGING_H
#define SQ_IMAGING_H

#define MAX_PIXEL_VAL 255

#include <stdio.h>

/** 
 * Allocate memory to an image buffer
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_alloc_img(float* img_buf, int rows, int cols);

/**
 * Linear scaling of image brightness
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_linear_scale(float* img_buf, int rows, int cols);

/**
 * Amplitude scaling of image brightness
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_amp_scale(float* img_buf, int rows, int cols);

/**
 * Power scaling of image brightness
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_power_scale(float* img_buf, int rows, int cols);

/**
 * No scaling of image, this is a no-op to be passed for the scale function pointer
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_no_scale(float* img_buf, int rows, int cols);

/**
 * Read signal data from input stream and generate a grayscale image matrix
 * @param instream input stream
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_read_img(FILE* instream, float* img_buf, int rows, int cols);

/**
 * Write image data to output buffer
 * @param outstream output stream
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_write_img(FILE* outstream, float* img_buf, int rows, int cols);

/**
 * Chop image edges, top+bottom or left+right
 * @param img_in input image
 * @param rows Number of rows in the input image
 * @param cols Number of columns in the input image
 * @param width_chop_fraction Fraction of width to discard
 * @param height_chop_fraction Fraction of height to discard
 */
int sq_imchop(float* img_in, float* img_out, int rows, int cols, float width_chop_fraction, float height_chop_fraction);

/**
 * Write image data formatted as a PGM/PNM image to output buffer
 * @param outstream output stream
 * @param img_buf float pointer to image buffer
 * @param rows Number of rows
 * @param cols Number of columns
 */
int sq_write_pnm(FILE* outstream, float* img_buf, int rows, int cols);

/**
 * Average the horizontal raster lines of the image buffer -
 * this sometimes reveals certain patterns that may otherwise evade a visual analysis
 * @param img_in float pointer to input image buffer
 * @param rows Number of rows in input image
 * @param cols Number of columns in input image
 * @param img_out float pointer to output image buffer
 * @param avglines Number of input raster lines to average into a single output row
 */
int sq_average_lines(float* img_in, int rows, int cols, float* img_out, int avglines);

#endif
