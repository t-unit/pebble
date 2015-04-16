#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void appmsg_in_dropped(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %i", reason);
}

static void message_handler(DictionaryIterator *iter, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received message.");
    Tuple *msg_type_tuple = dict_find(iter, PebbleMessageKeyType);
    Tuple *msg_value_tuple = dict_find(iter, PebbleMessageKeyValue);
    
    text_layer_set_text(text_layer, msg_value_tuple->value->cstring);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Enter something on your phone!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
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
  const bool animated = true;
  window_stack_push(window, animated);
}

static void setupMessages() {
    app_comm_set_sniff_interval(SNIFF_INTERVAL_NORMAL);
    
    app_message_register_inbox_received(message_handler);
    app_message_register_inbox_dropped(message_dropped);
    
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message set up.");
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
