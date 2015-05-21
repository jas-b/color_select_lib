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


#include "color_sel_lib.h"

const uint8_t CS_NUMCOLORS[] = {64, 11};
const bool CS_LOGGING = true;

const uint8_t CS_COLOR_OFFSET = 192;
const uint8_t CS_X_SIZE[] = {18, 36};
const uint8_t CS_Y_SIZE[] = {18, 36};
uint8_t choice;
uint8_t color_set = 0;
static char disp_text[24];

const uint8_t cs_order[][64] = {
  {
    0, 1, 2, 3, 4, 5, 6, 7, 
    16, 17, 18, 19, 20, 21, 22, 23, 
    32, 33, 34, 35, 36, 37, 38, 39, 
    48, 49, 50, 51, 52, 53, 54, 55, 
    56, 57, 58, 59, 60, 61, 62, 63, 
    40, 41, 42, 43, 44, 45, 46, 47, 
    24, 25, 26, 27, 28, 29, 30, 31, 
    8, 9, 10, 11, 12, 13, 14, 15,  
  }, 
  {
    0, 3, 11, 12, 
    15, 35, 48, 51, 
    56, 60, 63, 
/*
    1, 2, 4, 5, 6, 7, 
    8, 9, 10, 13, 14, 
    16, 17, 18, 19, 20, 21, 22, 23, 
    24, 25, 26, 27, 28, 29, 30, 31, 
    32, 33, 34, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 45, 46, 47, 
    49, 50, 52, 53, 54, 55, 
    57, 58, 59, 61, 62, 
*/
  }, 
};

const uint8_t cs_x[][64] = {
  {
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  0, 18*1, 18*2, 18*3, 18*4, 18*5, 18*6, 18*7, 
  }, 
  {
  0, 36*1, 36*2, 36*3, 
  0, 36*1, 36*2, 36*3, 
  0, 36*1, 36*2, 36*3, 
  }, 
};
const uint8_t cs_y[][64] = {
  {
  0, 0, 0, 0, 0, 0, 0, 0, 
  18*1, 18*1, 18*1, 18*1, 18*1, 18*1, 18*1, 18*1, 
  18*2, 18*2, 18*2, 18*2, 18*2, 18*2, 18*2, 18*2, 
  18*3, 18*3, 18*3, 18*3, 18*3, 18*3, 18*3, 18*3, 
  18*4, 18*4, 18*4, 18*4, 18*4, 18*4, 18*4, 18*4, 
  18*5, 18*5, 18*5, 18*5, 18*5, 18*5, 18*5, 18*5, 
  18*6, 18*6, 18*6, 18*6, 18*6, 18*6, 18*6, 18*6, 
  18*7, 18*7, 18*7, 18*7, 18*7, 18*7, 18*7, 18*7, 
  }, 
  {
  0, 0, 0, 0, 
  36*1, 36*1, 36*1, 36*1, 
  36*2, 36*2, 36*2, 36*2, 
  }, 
};

typedef struct cs_CSWindow {
	Window *window;
  Layer *draw_layer;
	TextLayer *select_layer;
	CSCloseHandler closeHandler;
	uint8_t default_color;
  bool full_palette;
} CSWindow;

void cs_draw_update_proc(Layer *layer, GContext *context);
void cswindow_destroy(CSWindow *window);
void cswindow_show(const CSWindow *window, bool animated);
void cs_clickConfigProvider(void * context);
void cs_back_click(ClickRecognizerRef recognizer, void * context);
void cs_select_click(ClickRecognizerRef recognizer, void * context);
void cs_up_click(ClickRecognizerRef recognizer, void * context);
void cs_down_click(ClickRecognizerRef recognizer, void * context);
void update_select(CSWindow *w);
void cs_info(const char * message);
void cs_warn(const char * message);
void cs_error(const char * message);

CSWindow * cswindow_create(uint8_t default_color, bool full_palette, 
						 CSCloseHandler closeHandler) {
	cs_info("Initializing CS window");
  
	CSWindow * w = (CSWindow*)malloc(sizeof(CSWindow));
  
  if (full_palette) {
    color_set = 0;
  } else {
    color_set = 1;
  }
  for (choice = 0; choice <= CS_NUMCOLORS[color_set]; choice++) {
    if (cs_order[color_set][choice] == default_color - CS_COLOR_OFFSET) break;
  }
  if (choice > CS_NUMCOLORS[color_set]) {
    choice = 0;
  }
	
	w->closeHandler = closeHandler;
	
	cs_info("Initializing CS window");
	w->window = window_create();
//	window_set_fullscreen(w->window, true);
	window_set_background_color(w->window, GColorWhite);
	window_set_click_config_provider_with_context(w->window,(ClickConfigProvider)cs_clickConfigProvider, w);

	Layer * windowLayer = window_get_root_layer(w->window);
//  GRect bounds = layer_get_bounds(windowLayer);

// create draw layer
  w->draw_layer = layer_create(GRect(0, 0, 144, 144));
  layer_set_update_proc(w->draw_layer, cs_draw_update_proc);
	layer_add_child(windowLayer, w->draw_layer);

// Create select layer
  w->select_layer = text_layer_create((GRect){.origin = {0, 144}, .size = {144, 24}});
//  text_layer_set_text(w->select_layer, "Select a color");
  snprintf(disp_text, 24, "%d", choice);
  text_layer_set_text(w->select_layer, disp_text);
  text_layer_set_text_alignment(w->select_layer, GTextAlignmentCenter);
  text_layer_set_background_color(w->select_layer, (GColor){.argb = cs_order[color_set][choice] + CS_COLOR_OFFSET});
  text_layer_set_text_color(w->select_layer, GColorWhite);
  layer_add_child(windowLayer, text_layer_get_layer(w->select_layer));
	
	cs_info("CS Window initialized");
	
	return w;
}

void cs_draw_update_proc(Layer *layer, GContext *context) {
//	CSWindow *w = (CSWindow*)context;
//  GRect bounds = layer_get_bounds(window_get_root_layer(w->window));

  cs_info("Drawing Colors");

  for (int i = 0; i < CS_NUMCOLORS[color_set]; i++) {
    graphics_context_set_fill_color(context, (GColor){.argb = cs_order[color_set][i] + CS_COLOR_OFFSET});
    graphics_fill_rect(context, (GRect){.origin = {cs_x[color_set][i], cs_y[color_set][i]}, .size = {CS_X_SIZE[color_set], CS_Y_SIZE[color_set]}}, 0, GCornerNone);
  }

  cs_info("Drawing Selection Box");

  char buff[10];
  snprintf(buff, 10, "%d", choice);
  cs_info(buff);

  if (choice < 32) {
    graphics_context_set_stroke_color(context, GColorWhite);
  } else {
    graphics_context_set_stroke_color(context, GColorBlack);
  }
  graphics_draw_rect(context,(GRect){.origin = {cs_x[color_set][choice], cs_y[color_set][choice]}, .size = {CS_X_SIZE[color_set], CS_Y_SIZE[color_set]}});
}

void cswindow_destroy(CSWindow *window) {
  cs_info("Destroying CS window");
  free(window->draw_layer);
  free(window->select_layer);
  free(window);
}

void cswindow_show(const CSWindow *window, bool animated) {
	cs_info("Showing CS window");
	window_stack_push(window->window, false);
}

void cs_switch_sets(ClickRecognizerRef recognizer, void * context) {
	CSWindow *w = (CSWindow*)context;
  if (color_set == 0) {
    color_set = 1;
  } else {
    color_set = 0;
  }
  if (choice > CS_NUMCOLORS[color_set]) {
    choice = 0;
  }
  update_select(w);
}

void cs_clickConfigProvider(void * context) {
  const int repeat = 500;
	window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)cs_back_click);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)cs_select_click);
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, (ClickHandler)cs_switch_sets, NULL);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)cs_up_click);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)cs_down_click);
//  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat, (ClickHandler)cs_up_click);
//  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat, (ClickHandler)cs_down_click);
}

void cs_back_click(ClickRecognizerRef recognizer, void * context) {
	cs_info("Back clicked");
//	CSWindow *w = (CSWindow*)context;
  cs_info("Popping window, no handler");
  window_stack_pop(true);
}

void cs_select_click(ClickRecognizerRef recognizer, void * context) {
	cs_info("Select clicked");
	CSWindow *w = (CSWindow*)context;
  cs_info("Popping window, with handler");
  char buff[10];
  snprintf(buff, 10, "%d", cs_order[color_set][choice]);
  cs_info(buff);
  window_stack_pop(true);
  if(w->closeHandler != NULL)
    w->closeHandler(cs_order[color_set][choice] + CS_COLOR_OFFSET);
}

void update_select(CSWindow *w) {
  if (choice < 32) {
    text_layer_set_text_color(w->select_layer, GColorWhite);
  } else {
    text_layer_set_text_color(w->select_layer, GColorBlack);
  }
  snprintf(disp_text, 24, "%d", choice);
  text_layer_set_text(w->select_layer, disp_text);

  text_layer_set_background_color(w->select_layer, (GColor){.argb = cs_order[color_set][choice] + CS_COLOR_OFFSET});
//  text_layer_set_text(text_layer[choice], "X");
  layer_mark_dirty(w->draw_layer);
}

void cs_up_click(ClickRecognizerRef recognizer, void * context) {
	CSWindow *w = (CSWindow*)context;
//  text_layer_set_text(select_layer, "Up");
	cs_info("Up clicked");
  if (choice == 0) {
    choice = CS_NUMCOLORS[color_set]-1;
  } else {
    choice--;
  }
  update_select(w);
}
void cs_down_click(ClickRecognizerRef recognizer, void * context) {
	CSWindow *w = (CSWindow*)context;
//  text_layer_set_text(select_layer, "Down");
	cs_info("Down clicked");
  if (choice == CS_NUMCOLORS[color_set]-1) {
    choice = 0;
  } else {
    choice++;
  }
  update_select(w);
}

void cs_info(const char * message) {
	if(CS_LOGGING)
		APP_LOG(APP_LOG_LEVEL_INFO, message);
}

void cs_warn(const char * message) {
	if(CS_LOGGING)
		APP_LOG(APP_LOG_LEVEL_WARNING, message);
}

void cs_error(const char * message) {
	if(CS_LOGGING)
		APP_LOG(APP_LOG_LEVEL_ERROR, message);
}