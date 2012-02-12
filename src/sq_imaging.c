/*******************************************************************************

  File:    sq_imaging.c
  Project: SETIkit
  Authors: Aditya Bhatt <aditya at adityabhatt dot org>
           Rob Ackermann <ackermann at seti dot org>

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
#include <stdio.h>
#include <stdlib.h>

#include "sq_imaging.h"
#include "sq_constants.h"

int sq_alloc_img(float* img_buf, int rows, int cols)
{
    img_buf = malloc(sizeof(float) * rows * cols);
    if(img_buf == NULL)
        return ERR_MALLOC;
}

int sq_no_scale(float* img_buf, int rows, int cols)
{

}

int sq_linear_scale(float* img_buf, int rows, int cols)
{
    unsigned int imgi;

    float imgvalf;

    float min = img_buf[0];
    float max = img_buf[0];

    for (imgi = 0; imgi < (rows * cols); imgi++)
    {
        if (img_buf[imgi] < min) min = img_buf[imgi];
        if (img_buf[imgi] > max) max = img_buf[imgi];
    }

    for (imgi = 0; imgi < (rows * cols); imgi++)
    {
        imgvalf = img_buf[imgi];
        imgvalf -= min;
        imgvalf *= ((float) MAX_PIXEL_VAL) / (max - min);
        img_buf[imgi] = imgvalf;
    }
}

int sq_power_scale(float* img_buf, int rows, int cols)
{
    unsigned int imgi;
    float imgvalf;

    float mean, stddev;
    float min, max;

    min = img_buf[0];

    for (imgi = 0; imgi < (rows * cols); imgi++)
        if (img_buf[imgi] < min) min = img_buf[imgi];
    if (min < 0.0)
        for (imgi = 0; imgi < (rows * cols); imgi++)
            img_buf[imgi] -= min;

    mean = 0.0;
    for (imgi = 0; imgi < (rows * cols); imgi++)
        mean += sqrt(img_buf[imgi]);
    mean /= (rows * cols);

    stddev = 0.0;
    for (imgi = 0; imgi < (rows * cols); imgi++)
        stddev += (sqrt(img_buf[imgi]) - mean) * (sqrt(img_buf[imgi]) - mean);
    stddev = sqrt(stddev / (rows * cols));

    min = mean - stddev;
    if (min < 0.0) min = 0.0;
    min = min * min;
    max = mean + (2.3 * stddev);
    max = max * max;

    for (imgi = 0; imgi < (rows * cols); imgi++)
    {
        imgvalf = img_buf[imgi];
        imgvalf -= min;
        imgvalf *= ((float) MAX_PIXEL_VAL) / (max - min);
        img_buf[imgi] = imgvalf;
    }
}

int sq_read_img(FILE* instream, float* img_buf, int rows, int cols)
{
    unsigned int rowi;

    if (!((rows > 0) && (cols > 0)))
        return ERR_ARG_BOUNDS;

    for (rowi = 0; rowi < rows; rowi++)
    {
        if (!(fread(&img_buf[rowi*cols], sizeof(float), cols, instream) == cols))
        {
            rows = rowi;
            break;
        }
    }

    return rows;
}

int sq_write_img(FILE* outstream, float* img_buf, int rows, int cols)
{
    unsigned int rowi;
    
    if (!((rows > 0) && (cols > 0)))
        return ERR_ARG_BOUNDS;
    
    for (rowi = 0; rowi < rows; rowi++)
    {
        if (!(fwrite(&img_buf[rowi*cols], sizeof(float), cols, outstream) == cols))
        {
            rows = rowi;
            break;
        }
    }
    
    if (!(rows > 0))
    {
        return ERR_STREAM_WRITE;
    }
}


int sq_imchop(float* img_in, float* img_out, int rows, int cols, float width_chop_fraction, float height_chop_fraction)
{
    unsigned int rowi;
    unsigned int coli;
    
    unsigned int col_first = cols * width_chop_fraction;
    unsigned int col_last = cols * (1 - width_chop_fraction);
    unsigned int col_count = col_last - col_first;
    
    unsigned int row_first = rows * height_chop_fraction;
    unsigned int row_last = rows * (1 - height_chop_fraction);
    unsigned int row_count = row_last - row_first;

    float imgvalf;
    
    for (rowi = row_first; rowi < row_last; rowi++)
    {
        for(coli = col_first; coli < col_last; coli++)
        {
            img_out[col_count*(rowi - row_first) + (coli - col_first)] = img_in[cols*rowi + coli];;
        }
    }
}

int sq_write_pnm(FILE* outstream, float* img_buf, int rows, int cols)
{
    unsigned int imgi;
    
    float imgvalf;
    unsigned char imgvalb;

    // write pnm header
    fprintf(outstream, "P5\n");
    fprintf(outstream, "%u %u\n", cols, rows);
    fprintf(outstream, "%u\n", MAX_PIXEL_VAL);

    for (imgi= 0; imgi < rows * cols; imgi++)
    {
        imgvalf = img_buf[imgi];
        
        if (imgvalf < 0.0)
            imgvalf = 0.0;
        if (imgvalf > (float) MAX_PIXEL_VAL) 
            imgvalf = (float) MAX_PIXEL_VAL;
        
        imgvalb = (unsigned char) imgvalf;
        fwrite(&imgvalb, 1, 1, outstream);
    }
}

    
int sq_average_lines(float* img_in, int rows, int cols, float* img_out, int avglines)
{
    unsigned int rowi, coli, rowi_out;
    int rows_out = rows/avglines;
    float* average_row = calloc(cols, sizeof(float));

    rowi_out = 0;
    for(rowi = 0; rowi < rows; rowi++)
    {
        for(coli = 0; coli < cols; coli++)
            average_row[coli] += img_in[cols*rowi + coli];

        if((rowi + 1) % avglines == 0)
        {
            for(coli = 0; coli < cols; coli++)
            {
                average_row[coli] /= avglines;
                img_out[cols*rowi_out + coli] = average_row[coli];
                average_row[coli] = 0;
            }
            rowi_out++;
        }
    }

    free(average_row);
}


