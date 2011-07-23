/*******************************************************************************

  File:    sq_signals.h
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
