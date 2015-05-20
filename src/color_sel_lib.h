/*
The MIT License (MIT)
Copyright (c) 2015 jas-b
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CS_WINDOW_H
#define CS_WINDOW_H

#include <pebble.h>

/**
 * The CS_Window type.
 *
 * This is the element that will be created and should be passed to the
 * interface functions.
 */
typedef struct cs_CSWindow CSWindow;

/**
 * The function signature for a handler that fires when the CSWindow is popped
 * from the stack by the user. It provides the text that was entered.
 */
typedef void (*CSCloseHandler)(const uint8_t select);

/**
 * Creates a new CSWindow, given the callback function.
 *
 *   void myCloseHandler(const uint8_t selected_color) {
 *      // Do something
 *   }
 *
 *   CSWindow * createCSWindow() {
 *      CSWindow * myCSWindow = cswindow_create(
 *         default_color, full_palette, 
 *         (CSCloseHandler)myCloseHandler);
 *      return myCSWindow;
 *   }
 *
 * default_color  The default color number
 * full_palette  true to open with all 64 colors, false for reduced palette
 * closeHandler  The CloseHandler to fire when the color select window closes.
 * A pointer to a new CSWindow.
 */
CSWindow * cswindow_create(uint8_t default_color, bool full_palette, 
						 CSCloseHandler closeHandler);

/**
 * Destroys a COLSELWindow previously created by cswindow_create().
 *
 * @param window  The COLSELWindow to destroy.
 */
void cswindow_destroy(CSWindow * window);

/**
 * Places the Window on the window stack.
 *
 * This call is equivalent to:
 *   window_stack_push(myCOLSELWindow->window, animated);
 *
 * @param window  The COLSELWindow to display.
 * @param animated  Whether to use the window_stack_push() animation.
 */
void cswindow_show(const CSWindow * window, bool animated);

/**
 * Sets the default color.
 *
 * @param window  The Window whose text to set.
 * @param default_col  A pointer to the default color.
 */
void cswindow_set_color(CSWindow * window, const uint8_t * default_col);

#endif
