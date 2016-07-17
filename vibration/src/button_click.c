#include <pebble.h>

//create our main window and our text layer before they are used
static Window *window;
static TextLayer *text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //set the text to "Double Vibe"
  text_layer_set_text(text_layer, "Double Vibe");
  
  //tell the watch to perform the double vibration pattern
  vibes_double_pulse();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
   //set the text to "Custom Vibes"
  text_layer_set_text(text_layer, "Custom Vibes");
  
  //create an array of int to represent the number of milliseconds to vibrate for; eg 200 ms on, 100 ms off, then 400 ms on
  static uint32_t segments[] = { 200, 100, 400 };
  
  //create the pattern using our segments array
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  
  //tell the watch to perform our custom pattern
  vibes_enqueue_custom_pattern(pat);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //set the text layer to "Short Vibe"
  text_layer_set_text(text_layer, "Short Vibe");
  
  //tell the watch to perform the short pulse pattern
  vibes_short_pulse();
}

static void click_config_provider(void *context) {
  
  //register handlers for each of the button presses
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  //create a main window and give it the bounds of the screen
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //set up our text layer
  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  //set the default text of our text layer
  text_layer_set_text(text_layer, "Press a button");
  //change our text layers alignment
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  //finally add it to our main window so the user can see it
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  // when our main window is destroyed, we need to remove all of the things on it when we destroy it
  text_layer_destroy(text_layer);
}

static void init(void) {
  //when we first initialize we need to do some setup, first create a window
  window = window_create();
  //then we set its click providers to be the ones we set up on line 39 - 45
  window_set_click_config_provider(window, click_config_provider);
  //then set up the window handlers for loading and unloading
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  //then we push the window onto the screen and tell it to animate on
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  // when our app stops we have to destroy all our windows
  window_destroy(window);
}

int main(void) {
  // this is our main loop, we want to first initialize everything
  init();

  //then print to the console so we can see the app has started
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  //then our main loop runs which handles all of our uer interface, vibrations, and button presses
  app_event_loop();
  
  //finally when the app is done, destroy everything! Muhaha
  deinit();
}