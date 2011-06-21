#ifndef SQ_IMAGING_H
#define SQ_IMAGING_H

#define MAX_PIXEL_VAL 255

#include <stdio.h>

int sq_alloc_img(float* img_buf, int rows, int cols);

int sq_linear_scale(float* img_buf, int rows, int cols);
int sq_power_scale(float* img_buf, int rows, int cols);
int sq_no_scale(float* img_buf, int rows, int cols);

int sq_read_img(FILE* instream, float* img_buf, int rows, int cols);
int sq_write_img(FILE* outstream, float* img_buf, int rows, int cols);
int sq_chop(float* img_in, float* img_out, int rows, int cols, float width_chop_fraction, float height_chop_fraction);
int sq_write_pnm(FILE* outstream, float* img_buf, int rows, int cols);

#endif