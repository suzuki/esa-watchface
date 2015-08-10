#include <pebble.h>
#include <stdlib.h>

static Window *window;
static TextLayer *s_watch_layer;
static TextLayer *s_tweet_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static GFont *s_custom_font;
static char *s_tweets[] = {
  "ｱﾁｰ",
  "ﾕ!",
  "ﾆﾞｯ!",
  "ﾌﾞﾝﾁｮｰ",
  "ﾓﾁ!",
  "ﾋﾟｰ",
  "ﾏﾝﾒﾝﾐ!",
  "ﾊﾞｰﾝ",
  "ｽｲｰ",
  "ﾔｯﾊﾛｰ",
  "ｺﾞﾛｺﾞﾛ",
  "ｱｷﾀｰ",
  "ｵｷﾀ?",
};

static void update_tweet() {
  int type_rand = rand() % 2;

  if (type_rand == 0) {
    text_layer_set_text(s_tweet_layer, "#ﾄﾉｺﾄ");
  } else {
    int text_rand = rand() % 12; // count of s_tweets
    text_layer_set_text(s_tweet_layer, s_tweets[text_rand]);
  }
}

static void update_time() {
  time_t tmp = time(NULL);
  struct tm *tick_time = localtime(&tmp);

  static char buffer[] = "00:00";

  if (clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // update tweet
  if (tick_time->tm_min % 30 == 0) {
    update_tweet();
  }

  // for screenshot
  //text_layer_set_text(s_watch_layer, "06:10");

  text_layer_set_text(s_watch_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_IMAGE);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  s_watch_layer = text_layer_create((GRect) { .origin = { 0, 100 }, .size = { bounds.size.w, 30 } });
  text_layer_set_font(s_watch_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_background_color(s_watch_layer, GColorClear);
  text_layer_set_text_color(s_watch_layer, GColorWhite);
  text_layer_set_text_alignment(s_watch_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_watch_layer));

  s_tweet_layer = text_layer_create((GRect) { .origin = {0, 148}, .size = {bounds.size.w, 20}});
  text_layer_set_text_alignment(s_tweet_layer, GTextAlignmentCenter);

  s_custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROUNDED_MPLUS_FONT_14));
  text_layer_set_font(s_tweet_layer, s_custom_font);
  text_layer_set_text(s_tweet_layer, "#ﾄﾉｺﾄ");
  layer_add_child(window_layer, text_layer_get_layer(s_tweet_layer));

  update_time();
}

static void window_unload(Window *window) {
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  text_layer_destroy(s_watch_layer);
  text_layer_destroy(s_tweet_layer);
  fonts_unload_custom_font(s_custom_font);
}

static void init(void) {
  srand((unsigned int)time(NULL));

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  const bool animated = true;
  window_stack_push(window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
