# color_select_lib

color selector library for use with pebble time

# Usage

#### 1. Add color_sel_lib.h and color_sel_lib.c to your Pebble project.
```c
#include "color_sel_lib.h"
```
#### 2. Define a handler to get the entered text:
```c
void myCloseHandler(const char * text) {
  // Do something
}
```
#### 3. Create the window and show it:
```c
CSWindow * myCSWindow = cswindow_create(
  color, false, 
  (CSCloseHandler)handle_CS_close_edit);

cswindow_show(myCSWindow, true);
```

## Structures
### CSWindow
This holds information about the CS Window. It is created with ```cswindow_create()``` and must be passed to the other interface functions.

### Handlers
### void (*CSCloseHandler)(const char * text)
This is a handler that is fired when the user accepts their entered text and closes the window.  It is used by ```cswindow_create()```.

|Parameter|Description|
|---|---|
|text|The entered text.|

## Functions
### cswindow_create
```c
CSWindow * cswindow_create(
    int default_color, bool full_palette,
    CSCloseHandler closeHandler)
```
Creates a new CSWindow, given the default color, full palette and callback function.

The user can toggle between full colors or a smaller subset by holding the SELECT buttons.

|Parameter|Description|
|---|---|
|**default_color**|The default color that the window will try to match. This may be null.|
|**full_palette**|true to use all 64 color, false to use a subset of 11 main colors. This may be null.|
|**closeHandler**|The CSCloseHandler to fire when the keyboard closes. This may be null.|

#### Returns
A new ```CSWindow``` structure.

### cswindow_destroy
```c
void cswindow_destroy(CSWindow * window)
```
Destroys a ```CSWindow``` previously created by ```cswindow_create()```.

|Parameter|Description|
|---|---|
|**window**|The ```CSWindow``` to destroy.|

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

