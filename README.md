## color_select_lib

color selector library for use with pebble time

## Usage

#### 1. Add color_sel_lib.h and color_sel_lib.c to your Pebble project.
```c
#include "color_sel_lib.h"
```
#### 2. Define a handler to get the entered color:
```c
void handle_CS_close(int color_argb) {
  GColor color = (GColor){.argb = color_argb};
  // Do something
  
  // or use directly
  text_layer_set_background_color(text_layer_name, (GColor){.argb = color_argb});
}
```
#### 3. Create the window and show it:
```c
CSWindow * myCSWindow = cswindow_create(
  default_color, false, 
  (CSCloseHandler)handle_CS_close);

cswindow_show(myCSWindow, true);
```

|Parameter|Description|
|---|---|
|**default_color**|The default color that the window will try to match.|
|**full_palette**|true to use all 64 color, false to use a subset of 11 main colors. The user can toggle between full colors or a smaller subset by holding the SELECT button.|
|**closeHandler**|The CSCloseHandler to fire when the keyboard closes.|

## Structures
### CSWindow
This holds information about the CS Window. It is created with ```cswindow_create()``` and must be passed to the other interface functions.

### Handlers
### void (*CSCloseHandler)(const int * color_argb)
This is a handler that is fired when the user accepts the color and closes the window.  It is used by ```cswindow_create()```.

|Parameter|Description|
|---|---|
|color_argb|The selected color (integer .argb value).|

## Functions
### cswindow_create
```c
CSWindow * cswindow_create(
    int default_color, bool full_palette,
    CSCloseHandler closeHandler)
```
Creates a new CSWindow, given the default color, full palette switch and callback function.

#### Returns
A new ```CSWindow``` structure.

### cswindow_show
```c
void cswindow_show(const CSWindow * window, bool animated)
```
Places the ```CSWindow``` on the window stack.

This call is equivalent to:
```c
window_stack_push(myCSWindow->window, animated);
```
|Parameter|Description|
|---|---|
|**window**|The ```CSWindow``` to display.|
|**animated**|Whether to use the ```window_stack_push()``` animation.|

