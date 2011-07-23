/*******************************************************************************

  File:    sq_constants.h
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

#ifndef SQ_CONSTANTS_H
#define SQ_CONSTANTS_H

#include <inttypes.h>
#include <math.h>

// Error codes
#define ERR_ARG_BOUNDS -1
#define ERR_STREAM_OPEN -2
#define ERR_STREAM_CLOSE -3
#define ERR_STREAM_READ -4
#define ERR_STREAM_WRITE -5
#define ERR_MALLOC -6
#define ERR_UNKNOWN_OPTION -7
#define ERR_UNKNOWN_WINDOW -8

// Some constants
#define MAX_SMPLS_LEN 134217728
#define TOTAL_BYTES_TRIGGER_VAL 1000000000
#define SMPLS_PER_READ 1000000
#define MAX_WNDW_LEN 134217728
#define SQ_STAGE1_FFT_LEN 4096
#define MAX_ZOOM_LEN 134217728
#define ZOOM_OUTPUT_BFR_LEN 1024

// Math constants
#define PI 3.1415926535897932384626433832795

// Array indices
#define REAL 0
#define IMAG 1

typedef float cmplx[2];
            
#endif