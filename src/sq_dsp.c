/*******************************************************************************

  File:    sq_dsp.c
  Project: SETIkit
  Authors: Aditya Bhatt <aditya at adityabhatt dot org>
           Rob Ackermann
       Gerry Harp <gharp @ seti dot org>

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
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <fftw3.h>

#include "sq_constants.h"
#include "sq_dsp.h"
#include "sq_utils.h"
#include "sq_windows.h"

int sq_abs(FILE* instream, FILE* outstream, unsigned int in_length)
{
    float *in_buffer;
    unsigned int smpli;

    if ((in_length < 2) || (in_length >= MAX_SMPLS_LEN))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length ; smpli++)
        {
            in_buffer[(smpli<<1)+0] =
                sqrt(  (in_buffer[(smpli<<1)+0] * in_buffer[(smpli<<1)+0]) +
                       (in_buffer[(smpli<<1)+1] * in_buffer[(smpli<<1)+1])   );
            in_buffer[(smpli<<1)+1] = 0.0;
        }

        fwrite(in_buffer, sizeof(cmplx), in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_power(FILE* instream, FILE* outstream, unsigned int in_length)
{
    float *in_buffer;
    unsigned int smpli;

    if ((in_length < 2) || (in_length >= MAX_SMPLS_LEN))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length ; smpli++)
        {
            in_buffer[(smpli<<1)+0] =
                (in_buffer[(smpli<<1)+0] * in_buffer[(smpli<<1)+0]) +
                (in_buffer[(smpli<<1)+1] * in_buffer[(smpli<<1)+1]);
            in_buffer[(smpli<<1)+1] = 0.0;
        }

        fwrite(in_buffer, sizeof(cmplx), in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_crossmultiply(FILE* instream1, FILE* instream2, FILE* outstream, unsigned int in_length) 
{
    float *bfr1, *bfr2;
    float real = 0, imag = 0;
    unsigned int smpli;
    unsigned int buf_count = 0;

    if ((in_length < 2) || (in_length >= MAX_SMPLS_LEN))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    bfr1 = malloc(in_length * sizeof(cmplx));
    if (bfr1 == NULL) return ERR_MALLOC;

    bfr2 = malloc(in_length * sizeof(cmplx));
    if (bfr2 == NULL) return ERR_MALLOC;

    while (
        (fread(bfr1, sizeof(cmplx), in_length, instream1) == in_length) &&
        (fread(bfr2, sizeof(cmplx), in_length, instream2) == in_length)
    )
    {
        ++buf_count;

        for (smpli = 0; smpli < in_length; smpli++)
        {
            real =
                (bfr1[(smpli<<1)+0]*bfr2[(smpli<<1)+0]) -
                (bfr1[(smpli<<1)+1]*bfr2[(smpli<<1)+1]);

            imag =
                (bfr1[(smpli<<1)+0]*bfr2[(smpli<<1)+1]) +
                (bfr2[(smpli<<1)+1]*bfr1[(smpli<<1)+0]);

            // re-use a buffer to save memory
            bfr1[(smpli<<1)+0] = real;
            bfr1[(smpli<<1)+1] = imag;
        }

        fwrite(bfr1, sizeof(cmplx), in_length, outstream);

        if (buf_count%100 == 1) fprintf(stderr, "Completed cycle %i\n", buf_count);
    }

    free(bfr1);
    free(bfr2);

    return 0;
}

int sq_sum(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int num_to_sum)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    float *in_buffer;
    float *sum_bfr;
    unsigned int rasteri;
    unsigned int smpli;
    unsigned int raster_count;
    int first_time = 1;
    int schedule_shutdown = 0;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    sum_bfr = malloc(in_length * sizeof(cmplx));
    if (sum_bfr == NULL) return ERR_MALLOC;

    for (;;) // loop until break
    {
        raster_count = 0;

        // zero the array containing sum
        for (smpli = 0; smpli < in_length; ++smpli)
        {
            sum_bfr[(smpli<<1)+0] = 0.0;
            sum_bfr[(smpli<<1)+1] = 0.0;
        }

        for (rasteri = 0; rasteri < num_to_sum; ++rasteri)
        {
            // read a new raster line of data
            if (fread(in_buffer, sizeof(cmplx), in_length, instream) == in_length)
            {
                ++raster_count;

                // sum this raster with previous
                for (smpli = 0; smpli < in_length; smpli++)
                {
                    sum_bfr[(smpli<<1)+0] += in_buffer[(smpli<<1)+0];
                    sum_bfr[(smpli<<1)+1] += in_buffer[(smpli<<1)+1];
                }
            }
            else
            {
                // we are out of data, get ready to shut down
                schedule_shutdown = 1;
            }
        }

        // if there is only one output raster, send it no matter what
        // if there have been other output rasters before, don't send unless
        // this one is complete
        if (first_time || raster_count == num_to_sum)
            fwrite(sum_bfr, sizeof(cmplx), in_length, outstream);
        first_time = 0;

        // if we are out of data, then break
        if (schedule_shutdown) break;
    }

    free(sum_bfr);
    free(in_buffer);

    return 0;
}

int sq_bandpass( FILE* instream, FILE* outstream, unsigned int in_length, char bp_file[])
{
fprintf(stderr, "sq_bandpass not yet implemented.");
return -1;
    char line[100] = "";
    float *in_buffer;
    float *wndw_bfr;
    unsigned int smpli;
    int count;

    if (!((in_length >= 2) && (in_length <= MAX_WNDW_LEN)))
    {
        fprintf(stderr, "Window lengths must be between 2 and %u\n", MAX_WNDW_LEN);
        return ERR_ARG_BOUNDS;
    }

    wndw_bfr = malloc(in_length * sizeof(float));
    if (wndw_bfr == NULL) return ERR_MALLOC;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    // Read bandpass file
    FILE *bp;
    bp = fopen(bp_file, "r");
    fprintf(stderr, "*** $s", bp_file);
    if ( bp != NULL )
    {
	count = 0;
        while(fgets(line, sizeof line, bp) != NULL )
	{
            fputs(line, stdout);
	    count++;
            printf("Bandpass length = %i.\n", count);
	    if (count > in_length)
            {
                sq_error_print("ERROR: Bandpass file too long\n");
		return ERR_ARG_BOUNDS;
            }
        }
        fclose(bp);

        printf("Bandpass length = %i.\n", count);
	if (count < in_length)
        {
            sprintf(line, "Bandpass length = %i.\n", count);
            sq_error_print(line);
            sq_error_print("ERROR: Bandpass file too short\n");
            return ERR_ARG_BOUNDS;
        }
    }
    else
    {
        sprintf(line, "ERROR: Bandpass file \"%s\" does not exist\n", bp_file);
        sq_error_print(line);
        return ERR_ARG_BOUNDS;
    }

}

// NOTE: This function does NOT do overlaps!
int sq_window( FILE* instream, FILE* outstream, unsigned int in_length, char* window_name)
{
    float *wndw_bfr;
    float *in_buffer;

    if (!((in_length >= 2) && (in_length <= MAX_WNDW_LEN)))
    {
        fprintf(stderr, "Window lengths must be between 2 and %u\n", MAX_WNDW_LEN);
        return ERR_ARG_BOUNDS;
    }

    wndw_bfr = malloc(in_length * sizeof(float));
    if (wndw_bfr == NULL) return ERR_MALLOC;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    // Make window 
    int status = sq_make_window_from_name(wndw_bfr, in_length, window_name);
    if (status < 0)
        return status;

    unsigned int bfri;
    // remember that in_buffer is declared as floats, but read as complex
    while (fread(in_buffer, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (bfri = 0; bfri < in_length; bfri++)
        {
            in_buffer[(bfri<<1)+0] *= wndw_bfr[bfri];
            in_buffer[(bfri<<1)+1] *= wndw_bfr[bfri];
        }
        fwrite(in_buffer, sizeof(cmplx), in_length, outstream);
    }

    free(wndw_bfr);
    free(in_buffer);

    return 0;
}

int sq_component(FILE* instream, FILE* outstream, unsigned int in_length, int component)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if ((component != 0) && (component != 1))
    {
        fprintf(stderr, "Component must be 0 (real) or 1 (imag).\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    int i;
    cmplx *in_buffer;
    float *out_buffer;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    out_buffer = malloc(in_length * sizeof(float));
    if (out_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (i = 0; i < in_length; i++)
            out_buffer[i] = in_buffer[i][component];
        fwrite(out_buffer, sizeof(float), in_length, outstream);
    }

    free(out_buffer);
    free(in_buffer);

    return 0;
}

int sq_real(FILE* instream, FILE* outstream, unsigned int in_length)
{
    return sq_component(instream, outstream, in_length, REAL);
}

int sq_imag(FILE* instream, FILE* outstream, unsigned int in_length)
{
    return sq_component(instream, outstream, in_length, IMAG);
}

int sq_fft(FILE* instream, FILE* outstream, unsigned int in_length,
           unsigned char is_conjugated, unsigned char is_measured,
           unsigned char inverse)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    fftwf_complex *fft_bfr;
    fftwf_plan plan;

    int i;

    fft_bfr = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * in_length);

    plan = fftwf_plan_dft_1d(in_length,
                             (fft_bfr),
                             (fft_bfr),
                             (inverse ? FFTW_BACKWARD : FFTW_FORWARD),
                             (is_measured ? FFTW_MEASURE : FFTW_ESTIMATE));

    while (fread(fft_bfr, sizeof(fftwf_complex), in_length, instream) == in_length)
    {
        if (is_conjugated)
            for (i = 0; i < in_length; i++)
                fft_bfr[i][1] = -fft_bfr[i][1];  // conjugate

        if (inverse)
        {
            // move channels back to their original positions before the fft, so  that ifft gets what it expects
            sq_channelswap(fft_bfr, in_length);

            // perform ifft
            fftwf_execute(plan);

            // inverse fft (effectively) multiples output by N, so take this out
            float norm = 1.0f / in_length; // multiplies are faster than divides
            for (i = 0; i < in_length; i++)
            {
                fft_bfr[i][0] = fft_bfr[i][0] * norm;
                fft_bfr[i][1] = fft_bfr[i][1] * norm;
            }
        }
        else
        {
            // perform fft
            fftwf_execute(plan);

            // write negative channels on the left, and then positive channels on the right
            sq_channelswap(fft_bfr, in_length);
        }

        fwrite(&fft_bfr[0], sizeof(fftwf_complex), in_length , outstream);
    }

    fftwf_destroy_plan(plan);
    fftwf_free(fft_bfr);


    return 0;
}

int sq_offset(FILE* instream, FILE* outstream, unsigned int in_length, float real_delta, float imag_delta)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    unsigned int smpli;
    float *in_buffer;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, 8, in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length; smpli++)
        {
            in_buffer[(smpli<<1)+0] += real_delta;
            in_buffer[(smpli<<1)+1] += imag_delta;
        }

        fwrite(in_buffer, 8, in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_subavg(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    float *in_buffer;
    unsigned int smpli;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, 8, in_length, instream) == in_length)
    {
        // compute average value
        double sumr = 0;
        double sumi = 0;
        for (smpli = 0; smpli < in_length; smpli += 1)
        {
            sumr += in_buffer[(smpli<<1)+0];
	    sumi += in_buffer[(smpli<<1)+1];
        }
	float favgr = (float)(sumr/in_length);
	float favgi = (float)(sumi/in_length);

        // subtract it off
        for (smpli = 0; smpli < in_length; smpli += 1)
        {
            in_buffer[(smpli<<1)+0] -= favgr;
	    in_buffer[(smpli<<1)+1] -= favgi;
        }

        fwrite(in_buffer, sizeof(cmplx), in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_conjugate(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    float *in_buffer;
    unsigned int smpli;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    while (fread(in_buffer, 8, in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length; smpli += 1)
            in_buffer[(smpli<<1)+1] *= -1.0;

        fwrite(in_buffer, sizeof(cmplx), in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_scaleandrotate(FILE* instream, FILE* outstream, unsigned int in_length, float scale_factor, float radians)
{
    if (in_length <= 0)
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    unsigned int smpli;
    float *in_buffer;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    float re, im;
    while (fread(in_buffer, 8, in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length; smpli++)
        {
            re = in_buffer[(smpli<<1)+0];
            im = in_buffer[(smpli<<1)+1];
            in_buffer[(smpli<<1)+0] = scale_factor * (re * cos(radians) - im * sin(radians));
            in_buffer[(smpli<<1)+1] = scale_factor * (im * cos(radians) + re * sin(radians));
        }

        fwrite(in_buffer, 8, in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

int sq_scale(FILE* instream, FILE* outstream, unsigned int in_length, float scale_factor)
{
    return sq_scaleandrotate(instream, outstream, in_length, scale_factor, 0.0);
}

int sq_rotate(FILE* instream, FILE* outstream, unsigned int in_length, float radians)
{
    return sq_scaleandrotate(instream, outstream, in_length, 1.0, radians);
}

int sq_mix(FILE* instream, FILE* outstream, unsigned int in_length, float radians)
{
    if (in_length <= 0)
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (abs(radians) > 2.0 * M_PI)
    {
        sq_error_print("Warning: |Radians| > 2*PI, aliasing will occur.\n");
    }

    unsigned int smpli;
    float *in_buffer;
    float angle = 0.0;

    const float TWO_PI = 2.0 * M_PI;

    radians = radians * -1.0;

    in_buffer = malloc(in_length * sizeof(cmplx));
    if (in_buffer == NULL) return ERR_MALLOC;

    float re, im;
    while (fread(in_buffer, 8, in_length, instream) == in_length)
    {
        for (smpli = 0; smpli < in_length; smpli++)
        {
            angle += radians;
            angle = (angle > TWO_PI) ? (angle - TWO_PI) : angle;
            angle = (angle < -TWO_PI) ? (angle + TWO_PI) : angle;

            re = in_buffer[(smpli<<1)+0];
            im = in_buffer[(smpli<<1)+1];
            in_buffer[(smpli<<1)+0] = (re * (float)cos(angle) - im * (float)sin(angle));
            in_buffer[(smpli<<1)+1] = (im * (float)cos(angle) + re * (float)sin(angle));
        }

        fwrite(in_buffer, 8, in_length, outstream);
    }

    free(in_buffer);

    return 0;
}

void init_window(float* wndwbfr, unsigned int wndwlen, unsigned int folds)
{
    unsigned int wndwi;
    unsigned int si;

    unsigned int sinusoids = (folds / 2) + 1;

    float h;

    for (wndwi = 0; wndwi < wndwlen; wndwi++)
    {
        h = 0.5 * (1.0 - cos((2.0 * M_PI * (float) wndwi) / (float)(wndwlen - 1)));
        wndwbfr[wndwi] = 0.0;
        for (si = 0; si < sinusoids; si++)
            wndwbfr[wndwi] += cos(((-2.0 * M_PI) / (float) wndwlen) *
                                  (si * (((float) wndwi) - (((float)(wndwlen - 1)) / 2.0))));
        wndwbfr[wndwi] *= h;
        wndwbfr[wndwi] /= sinusoids;
    }
}

int sq_wola(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int folds,
            unsigned int overlap, unsigned char is_window_dump)
{
    if (!((in_length >= 2) && (in_length <= MAX_ZOOM_LEN)))
    {
        fprintf(stderr, "Zoom length must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (folds < 1)
    {
        sq_error_print("Number of folds must be greater than 1.\n");
        return ERR_ARG_BOUNDS;
    }
    if (!((overlap < 1) && (overlap >= in_length)))
    {
        sq_error_print("Overlap must be in range (1, in_length).\n");
        return ERR_ARG_BOUNDS;
    }

    unsigned int wndwlen;

    float *wndwbfr;
    cmplx *readbfr;
    cmplx *smplbfr;
    cmplx *fftbfr;

    unsigned int wndwi, readi, smpli, ffti;
    unsigned int readlen;

    wndwlen = folds * in_length;
    wndwbfr = malloc(wndwlen * sizeof(float));
    if (wndwbfr == NULL) return ERR_MALLOC;

    init_window(wndwbfr, wndwlen, folds);


    if (is_window_dump)
    {
        for (wndwi = 0; wndwi < wndwlen; wndwi++)
            fprintf(outstream, "%e\n", wndwbfr[wndwi]);
        free(wndwbfr);
        return 0;
    }

    readlen = in_length;
    if (overlap == 25)
        readlen = (in_length * 3) / 4;
    if (overlap == 50)
        readlen = (in_length * 2) / 4;

    readbfr = malloc(readlen * sizeof(cmplx));
    if (readbfr == NULL) return ERR_MALLOC;

    smplbfr = malloc(wndwlen * sizeof(cmplx));
    if (smplbfr == NULL) return ERR_MALLOC;

    fftbfr = malloc(in_length * sizeof(cmplx));
    if (fftbfr == NULL) return ERR_MALLOC;

    // initially fill the sample buffer to satisfy the first weight,
    // overlap, and add
    if (!(fread(smplbfr, sizeof(cmplx), wndwlen, instream) == wndwlen))
    {
        free(fftbfr);
        free(smplbfr);
        free(readbfr);
        free(wndwbfr);
        return ERR_STREAM_READ;
    }

    smpli = 0;

    for (;;)
    {
        for (ffti = 0; ffti < in_length; ffti++)
            fftbfr[ffti][0] = fftbfr[ffti][1] = 0.0;
        ffti = 0;
        for (wndwi = 0; wndwi < wndwlen; wndwi++)
        {
            fftbfr[ffti][0] += wndwbfr[wndwi] * smplbfr[smpli][0];
            fftbfr[ffti][1] += wndwbfr[wndwi] * smplbfr[smpli][1];
            smpli++;
            if (!(smpli < wndwlen)) smpli = 0;
            ffti++;
            if (!(ffti < in_length)) ffti = 0;
        }
        fwrite(fftbfr, sizeof(cmplx), in_length, outstream);
        if (!(fread(readbfr, sizeof(cmplx), readlen, instream) == readlen))
            break;
        for (readi = 0; readi < readlen; readi++)
        {
            smplbfr[smpli][0] = readbfr[readi][0];
            smplbfr[smpli][1] = readbfr[readi][1];
            smpli++;
            if (!(smpli < wndwlen)) smpli = 0;
        }
    }

    free(fftbfr);
    free(smplbfr);
    free(readbfr);
    free(wndwbfr);

    return 0;
}

int sq_pad(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN))
            && ((out_length >= 2) && (out_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (out_length <= in_length)
    {
        sq_error_print("Output array length must be greater than input length.\n");
        return ERR_ARG_BOUNDS;
    }

    float *input_bfr;
    float *output_bfr;

    output_bfr = calloc(out_length, sizeof(cmplx));
    if (output_bfr == NULL) return ERR_MALLOC;

    // find the insertion point for reading data
    int offset = (out_length - in_length) / 2; 

    // input_bfr points to the center section of the output buffer
    input_bfr = output_bfr + offset * 2; // each cmplx sample is 2 floats

    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
        // the input data is already surrounded by zeros, just write it out
        fwrite(output_bfr, sizeof(cmplx), out_length , outstream);
    }

    free(output_bfr);

    return 0;
}

int sq_fftflip(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    float *input_bfr;

    input_bfr = malloc(in_length * sizeof(cmplx));
    if (input_bfr == NULL) return ERR_MALLOC;

    unsigned int smpli;
    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
	for (smpli = 0; smpli < in_length/2; smpli++)
        {
            float temp0 = input_bfr[(smpli<<1) + 0];
            float temp1 = input_bfr[(smpli<<1) + 1];
	    input_bfr[(smpli<<1) + 0] = input_bfr[(smpli<<1) + in_length + 0];
	    input_bfr[(smpli<<1) + 1] = input_bfr[(smpli<<1) + in_length + 1];
	    input_bfr[(smpli<<1) + in_length + 0] = temp0;
	    input_bfr[(smpli<<1) + in_length + 1] = temp1;
        }
 
        fwrite(input_bfr, sizeof(cmplx), in_length , outstream);
    }

    free(input_bfr);

    return 0;
}

int sq_bin(FILE* instream, FILE* outstream, unsigned int in_length, unsigned int out_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN))
            && ((out_length >= 2) && (out_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (out_length >= in_length)
    {
        sq_error_print("Output array length must be less than input length.\n");
        return ERR_ARG_BOUNDS;
    }

    float *input_bfr;
    float *output_bfr;

    input_bfr = malloc(in_length * sizeof(cmplx));
    if (input_bfr == NULL) return ERR_MALLOC;

    output_bfr = malloc(out_length * sizeof(cmplx));
    if (output_bfr == NULL) return ERR_MALLOC;

    unsigned int bin_size;
    bin_size = in_length / out_length;

    unsigned int in_i = 0, out_i, start, stop;
    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
        // reinitialize output buffer to zero each iteration
        memset(output_bfr, 0, out_length * sizeof(cmplx));

        for (out_i = 0; out_i < out_length; out_i++)
        {
            start = out_i * bin_size;
            stop = (out_i + 1) * bin_size;
            for (in_i = start; in_i < stop; in_i++)
            {
                output_bfr[(out_i<<1) + REAL] += input_bfr[(in_i<<1) + REAL] / bin_size;
                output_bfr[(out_i<<1) + IMAG] += input_bfr[(in_i<<1) + IMAG] / bin_size;
            }
        }

        fwrite(output_bfr, sizeof(cmplx), out_length , outstream);
    }

    free(input_bfr);
    free(output_bfr);

    return 0;
}

int sq_chop(FILE* instream, FILE* outstream, unsigned int in_length, float chop_fraction)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (!((chop_fraction > 0) && (chop_fraction) < 0.5))
    {
        sq_error_print("Chop fraction must be nonzero, positive and less than 0.5\n");
        return ERR_ARG_BOUNDS;
    }

    float *input_bfr;
    float *output_bfr;

    int samples_to_discard = (int)((float)in_length * chop_fraction);
    int out_length = in_length - 2 * samples_to_discard;

    input_bfr = malloc(in_length * sizeof(cmplx));

    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
        fwrite(output_bfr, sizeof(cmplx), out_length , outstream);
    }

    free(input_bfr);

    return 0;
}

int sq_overlap2x(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }
    if (!((in_length/2)*2 == in_length))
    {
        fprintf(stderr, "For overlaps, input array length must be divisible by 2.\n");
        return ERR_ARG_BOUNDS;
    }


    float *input_bfr1;
    float *input_bfr2;
    float *temp;
    float *output_bfr;
    float *offset_output_bfr;
    unsigned int half_len = in_length/2;

    input_bfr1 = malloc(half_len * sizeof(cmplx));
    if (input_bfr1 == NULL) return ERR_MALLOC;

    input_bfr2 = malloc(half_len * sizeof(cmplx));
    if (input_bfr2 == NULL) return ERR_MALLOC;

    output_bfr = malloc(in_length * sizeof(cmplx));
    if (output_bfr == NULL) return ERR_MALLOC;

    // even thought output_bfr is a complex array, it is declared as
    // float. Hence we must count floats when computing memory offset
    // adding in_length offsets to middle of array
    offset_output_bfr = output_bfr + in_length;

    // read first half-raster from stream -- assume it works, we'll check second read below
    fread(input_bfr1, sizeof(cmplx), half_len, instream);

    while (fread(input_bfr2, sizeof(cmplx), half_len, instream) == half_len)
    {
        memcpy(output_bfr,        input_bfr1, half_len * sizeof(cmplx));
        memcpy(offset_output_bfr, input_bfr2, half_len * sizeof(cmplx));
        fwrite(output_bfr, sizeof(cmplx), in_length , outstream);

        // swap buffers
        temp = input_bfr1;
        input_bfr1 = input_bfr2;
        input_bfr2 = temp;
    }

    free(input_bfr1);
    free(input_bfr2);
    free(output_bfr);

    return 0;
}


int sq_ascii(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    int i;
    float *input_bfr;
    float *output_bfr;

    input_bfr = malloc(in_length * sizeof(cmplx));
    if (input_bfr == NULL) return ERR_MALLOC;

    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (i = 0; i < in_length; ++i)
        {
            fprintf(outstream, "%f, %f\n",
                    input_bfr[(i<<1) + 0], input_bfr[(i<<1) + 1]);
        }
    }

    free(input_bfr);

    return 0;
}

int sq_phase(FILE* instream, FILE* outstream, unsigned int in_length)
{
    if (!((in_length >= 2) && (in_length <= MAX_SMPLS_LEN)))
    {
        fprintf(stderr, "Array lengths must be between 2 and %u\n", MAX_SMPLS_LEN);
        return ERR_ARG_BOUNDS;
    }

    int i;
    float *input_bfr;
    float val;

    input_bfr = malloc(in_length * sizeof(cmplx));
    if (input_bfr == NULL) return ERR_MALLOC;

    while (fread(input_bfr, sizeof(cmplx), in_length, instream) == in_length)
    {
        for (i = 0; i < in_length; ++i)
        {
            // compute absolute value of complex sample
            val = input_bfr[(i<<1) + 0] * input_bfr[(i<<1) + 0] +
                  input_bfr[(i<<1) + 1] * input_bfr[(i<<1) + 1];
            val = sqrt(val);
            //fprintf(stderr, "Abs value = %f.\n", val);

            // divide each sample by its absolute value
            // avoid divide by zero; phase is undefined if val = 0, just leave it
            if (val > 0)
            {
                input_bfr[(i<<1) + 0] /= val;
                input_bfr[(i<<1) + 1] /= val;
            }
        }

        fwrite(input_bfr, sizeof(cmplx), in_length , outstream);
    }

    free(input_bfr);

    return 0;
}
