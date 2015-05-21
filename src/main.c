#include <pebble.h>

#include "color_sel_lib.h"

const uint8_t color_key = 0;
uint8_t color = 48 + 192;

static Window *window;
static TextLayer *text_layer;

static char disp_text[24];

static void window_load(Window *window);
static void click_config_provider(void *context);
static void click_handler(ClickRecognizerRef recognizer, void *context);
void handle_CS_close_edit(int selected_color);

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  window_set_background_color(window, (GColor){.argb = color});
  GRect bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create((GRect) { .origin = { 0, 70 }, .size = { bounds.size.w, 24 } });
  text_layer_set_text(text_layer, "Select a color");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorBlack );
  text_layer_set_text_color(text_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  APP_LOG(APP_LOG_LEVEL_INFO, "main window load");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
  window_single_click_subscribe(BUTTON_ID_UP,  click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, click_handler);
}

static void click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "create");

  CSWindow * myCSWindow = cswindow_create(
    color, false, (CSCloseHandler)handle_CS_close_edit);

  APP_LOG(APP_LOG_LEVEL_INFO, "show");

  cswindow_show(myCSWindow, true);

}

void handle_CS_close_edit(int selected_color) {
//  cswindow_destroy(myCSWindow);
  APP_LOG(APP_LOG_LEVEL_INFO, "selection");
  APP_LOG(APP_LOG_LEVEL_INFO, "back");
  snprintf(disp_text, 24, "%d", selected_color);
  APP_LOG(APP_LOG_LEVEL_INFO, disp_text);
  GColor Bcolor = (GColor){.argb = selected_color};
  window_set_background_color(window, Bcolor);
//  window_set_background_color(window, (GColor){.argb = color});
  text_layer_set_text(text_layer, disp_text);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
//  window_set_fullscreen(window, true);
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}