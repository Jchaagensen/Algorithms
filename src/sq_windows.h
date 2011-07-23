/*******************************************************************************

  File:    sq_windows.h
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

#ifndef SQ_WINDOWS_H
#define SQ_WINDOWS_H

/**
 * Writes a window's samples to a buffer of a given length
 * @param window_buffer The float array to which the window is to be written
 * @param length The length of the window/array
 * @param *window_function A function pointer to a window function
 */
void sq_make_window(float* window_buffer, unsigned int length, float (*window_function)(unsigned int, unsigned int));

/**
 * Writes a window's samples to a buffer of a given length
 * @param window_buffer The float array to which the window is to be written
 * @param length The length of the window/array
 * @param window_name A string containing the desired window's name
 * @return 0 on success, ERROR_UNKNOWN_WINDOW if a window with the specified name is not found
 */
int sq_make_window_from_name(float* window_buffer, unsigned int length, char* window_name);

/**
 * Returns a value on a Hann window
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_hann(unsigned int n, unsigned int length);

/**
 * Returns a value on a Hamming window
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_hamming(unsigned int n, unsigned int length);

/**
 * Returns a value on a Sine/Cosine window
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_cosine(unsigned int n, unsigned int length);

/**
 * Returns a value on a Lanczos (similar to sinc) window
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_lanczos(unsigned int n, unsigned int length);

/**
 * Returns a value on a Triangular window
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_triangular(unsigned int n, unsigned int length);

/**
 * Returns a value on a Gaussian window. The default sigma value is 0.4
 * @param n index at which value is needed
 * @param length length of window
 */
float sq_window_gaussian(unsigned int n, unsigned int length);

#endif