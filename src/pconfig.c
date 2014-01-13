/**
 * This code is provided under the Apache 2.0 license.
 * Please read the LICENSE.md file for more information
 * 
 * Copyright (c) 2013 Matthew Congrove (http://github.com/mcongrove)
 */
#include <pebble.h>

static int TIME_DISPLAY = 0;

Window *window;
TextLayer *label_layer_time;
char timeText[] = "00:00";

enum {
	KEY_TIME_DISPLAY = 0x0
};

static void set_time_display() {
	// Check if our time display property exists
	if (persist_exists(KEY_TIME_DISPLAY)) {
		// If so, read it in to a variable
		TIME_DISPLAY = persist_read_int(KEY_TIME_DISPLAY);
	}
	
	// If time display == 0, hide the display, otherwise show
	bool hide = TIME_DISPLAY == 0 ? true : false;
	
	// Hide or show the layer
	layer_set_hidden(text_layer_get_layer(label_layer_time), hide);
	
	// Output log message for testing
	APP_LOG(APP_LOG_LEVEL_INFO, "SELECTED TIME DISPLAY: %d", TIME_DISPLAY);
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	// Get our time display property
	Tuple *time_tuple = dict_find(iter, KEY_TIME_DISPLAY);
	
	// If exists, write the property to persistent storage
	time_tuple ? persist_write_int(KEY_TIME_DISPLAY, time_tuple->value->uint8) : false;
	
	// Handle showing or hiding the time display
	set_time_display();
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	char *time_format;
	
	if(clock_is_24h_style()) {
		time_format = "%R";
	} else {
		time_format = "%I:%M";
	}
	
	strftime(timeText, sizeof(timeText), time_format, t);
	
	text_layer_set_text(label_layer_time, timeText);
}

static void init() {
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_open(128, 0);
	
	window = window_create();
	window_set_background_color(window, GColorBlack);
	window_stack_push(window, true);
	
	Layer *window_layer = window_get_root_layer(window);
	
	// Add time layer
	label_layer_time = text_layer_create(GRect(0, 65, 144, 30));
	text_layer_set_text_color(label_layer_time, GColorWhite);
	text_layer_set_background_color(label_layer_time, GColorClear);
	text_layer_set_text_alignment(label_layer_time, GTextAlignmentCenter);
	text_layer_set_font(label_layer_time, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	layer_set_hidden(text_layer_get_layer(label_layer_time), true);
	layer_add_child(window_layer, text_layer_get_layer(label_layer_time));
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
	
	set_time_display();
}

static void deinit() {
	window_destroy(window);
	text_layer_destroy(label_layer_time);
	
	tick_timer_service_unsubscribe();
	app_message_deregister_callbacks();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}