#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void message_dropped(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %i", reason);
}

static void message_handler(DictionaryIterator *iter, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received message.");

    Tuple *t = dict_read_first(iter);
    text_layer_set_text(text_layer, t->value->cstring);
}

static void setupMessages() {
    app_comm_set_sniff_interval(SNIFF_INTERVAL_NORMAL);
    
    app_message_register_inbox_received(message_handler);
    app_message_register_inbox_dropped(message_dropped);
    
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message set up.");
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Enter on your phone!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  setupMessages();

  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

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
