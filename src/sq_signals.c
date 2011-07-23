/*******************************************************************************

  File:    sq_signals.c
  Project: SETIkit
  Authors: Aditya Bhatt <aditya at adityabhatt dot org>, 
           Gerry Harp <gharp at seti dot org>

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

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "sq_utils.h"
#include "sq_constants.h"
#include "sq_signals.h"

int sq_gen_sine(FILE* outstream, unsigned int nsamples, unsigned int length, unsigned int sin_arr_length, float wavelength, float SNR)
{
    if((nsamples <= 0) || (nsamples > MAX_SMPLS_LEN))
        return ERR_ARG_BOUNDS;
    
    unsigned int i;
    unsigned int smpli;
    unsigned int index;
    
    float* Sin;
    float* Cos;
    float* smpls_out;
    
    fprintf(stderr, "SNR is %f\n", SNR);

    // generate LUT's for sine and cosine
    Sin = malloc(sin_arr_length * sizeof(float));
    if(Sin == NULL)
        return ERR_MALLOC;
    Cos = malloc(sin_arr_length * sizeof(float));
    if(Cos == NULL)
        return ERR_MALLOC;
    smpls_out = malloc(nsamples* 2 * sizeof(float));
    if(smpls_out == NULL)
        return ERR_MALLOC;
    
    for (index = 0; index < sin_arr_length; ++index)
    {
        Sin[index] = (float)sin(index * 2 * PI / sin_arr_length);
        Cos[index] = (float)cos(index * 2 * PI / sin_arr_length);
    }    
    
    for (i = 0; i < length; ++i)
    {
        for (smpli = 0; smpli < nsamples; smpli++)
        {
            index = (int) ((float)(i*nsamples + smpli) / wavelength);
            index = index%sin_arr_length;

            smpls_out[(smpli<<1) + REAL] = Sin[index] * SNR + sq_randgaus();
            smpls_out[(smpli<<1) + IMAG] = Cos[index] * SNR + sq_randgaus();
        }
        fwrite(smpls_out, 8, nsamples, outstream);
    }
    
    free(Sin);
    free(Cos);
    free(smpls_out);
    
    return 0;
}
