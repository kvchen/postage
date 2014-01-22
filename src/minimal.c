#include "pebble.h"

Window *window;
TextLayer *text_date_layer;
TextLayer *text_hour_layer;
TextLayer *text_minute_layer;
Layer *rect_layer;

void rect_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 16, GCornersAll);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char hour_text[] = "00";
  static char minute_text[] = "00";
  static char date_text[] = "Xxxxxxxxx 00x 0000";

  char *hour_format;
  char *minute_format;

  strftime(date_text, sizeof(date_text), "%B %e, %Y", tick_time);
  text_layer_set_text(text_date_layer, date_text);


  if (clock_is_24h_style()) {
    hour_format = "%H";
  } else {
    hour_format = "%I";
  }
  minute_format = "%M";

  strftime(hour_text, sizeof(hour_text), hour_format, tick_time);
  strftime(minute_text, sizeof(minute_text), minute_format, tick_time);

  text_layer_set_text(text_hour_layer, hour_text);
  text_layer_set_text(text_minute_layer, minute_text);
}

/*
void handle_day_tick(struct tm *tick_time, TimeUnits units_changed) {

}
*/

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
}

void handle_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  text_date_layer = text_layer_create(GRect(0, 142, 144, 168-142));
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  /*
  GRect day_frame = GRect(55, 4, 34, 4);
  day_layer = layer_create(day_frame);
  layer_set_update_proc(day_layer, handle_day_tick);
  layer_add_child(window_layer, rect_layer);
  */

  GRect rect_frame = GRect(28, 24, 144-56, 168-58);
  rect_layer = layer_create(rect_frame);
  layer_set_update_proc(rect_layer, rect_layer_update_callback);
  layer_add_child(window_layer, rect_layer);

  text_hour_layer = text_layer_create(GRect(0, 30, 144, 168-30));
  text_layer_set_text_color(text_hour_layer, GColorBlack);
  text_layer_set_background_color(text_hour_layer, GColorClear);
  text_layer_set_font(text_hour_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_BOLD_44)));
  text_layer_set_text_alignment(text_hour_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_hour_layer));

  text_minute_layer = text_layer_create(GRect(0, 78, 144, 168-78));
  text_layer_set_text_color(text_minute_layer, GColorBlack);
  text_layer_set_background_color(text_minute_layer, GColorClear);
  text_layer_set_font(text_minute_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FUTURA_MEDIUM_44)));
  text_layer_set_text_alignment(text_minute_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_minute_layer));

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  layer_mark_dirty(window_layer);
  // TODO: Update display here to avoid blank display on launch?
}


int main(void) {
  handle_init();

  app_event_loop();
  
  handle_deinit();
}
