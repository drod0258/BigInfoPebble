#include <pebble.h>
#include "suncalc.h"

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  // user settings
  GColor BackgroundColor;
  GColor TextColor;
  GColor BackgroundColorDay;
  GColor TextColorDay;
  GColor BackgroundColorNight;
  GColor TextColorNight;
  bool NightTheme;
  bool ShowWeather;
  bool TemperatureUnit;
  int WeatherInterval;
  bool ShowDate;
  bool ShowDate2;
  bool AltDate;
  bool ShowSteps;
  bool ShowSun;
  bool ShowMoon;
  bool ShowPhoneBattery;
  bool PeriodicVibrate;
  bool BluetoothVibrate;
  char GpsLat[12];
  char GpsLon[12];
  // storage
  bool IsDay;
  int SunriseTime;
  int SunsetTime;
  int MoonPhase;
  int WeatherTemp;
  int WeatherIcon;
  int PhoneBattery;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_date2_layer;
static TextLayer *s_steps_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_weather_icon_layer;
static TextLayer *s_sunrise_layer;
static TextLayer *s_sunset_layer;
static TextLayer *s_moon_layer;
static TextLayer *s_bt_icon_layer;

// Custom fonts
static GFont s_time_font;
static GFont s_date_font;
static GFont s_info_font;
static GFont s_bt_font;
static GFont s_weather_font;

// Battery
static Layer *s_battery_layer;
static int s_battery_level;
static Layer *s_phone_battery_layer;

// Unobstructed area
static Layer *s_window_layer;

// Set default settings
static void prv_default_settings() {
  // user settings
  settings.BackgroundColorDay = GColorWhite;
  settings.TextColorDay = GColorBlack;
  settings.BackgroundColorNight = GColorBlack;
  settings.TextColorNight = GColorWhite;
  settings.BackgroundColor = settings.BackgroundColorDay;
  settings.TextColor = settings.TextColorDay;
  settings.NightTheme = false;
  settings.ShowDate = false;
  settings.ShowDate2 = false;
  settings.AltDate = false;
  settings.ShowWeather = false;
  settings.TemperatureUnit = false;
  settings.WeatherInterval = 3;
  settings.ShowSteps = false;
  settings.ShowSun = true;
  settings.ShowMoon = false;
  settings.ShowPhoneBattery = false;
  settings.PeriodicVibrate = false;
  settings.BluetoothVibrate = false;
  snprintf(settings.GpsLat, sizeof(settings.GpsLat), "40.7128");
  snprintf(settings.GpsLon, sizeof(settings.GpsLon), "-74.0060");
  // storage
  settings.IsDay=false;
  settings.SunriseTime=1;
  settings.SunsetTime=2359;
  settings.MoonPhase=29;
  settings.WeatherTemp=-99;
  settings.WeatherIcon=15;
  settings.PhoneBattery=0;
}

static char* weather_conditions[] = {
  "\U0000f00d", //  0  "wi_day_clear"
  "\U0000f00c", //  1  "wi_day_sunny_overcast"
  "\U0000f002", //  2  "wi_day_cloudy"
  "\U0000f041", //  3  "wi_cloud"
  "\U0000f014", //  4  "wi_fog"
  "\U0000f01c", //  5  "wi_sprinkle"
  "\U0000f0b5", //  6  "wi_sleet"
  "\U0000f019", //  7  "wi_rain"
  "\U0000f017", //  8  "wi_rain_mix"
  "\U0000f064", //  9  "wi_snow_wind"
  "\U0000f01b", // 10  "wi_snow"
  "\U0000f01a", // 11  "wi_showers"
  "\U0000f01b", // 12  "wi_snow"
  "\U0000f01e", // 13  "wi_thunderstorm"
  "\U0000f01d", // 14  "wi_storm_showers"
  "\U0000F07B", // 15  "unknown"
  "\U0000f02e", // 16  "wi_night_clear"
  "\U0000f081", // 17  "wi_night_alt_partly_cloudy"
  "\U0000f086", // 18  "wi_night_alt_cloudy"
  "\U0000f041", // 19  "wi_cloud"
  "\U0000f014", // 20  "wi_fog"
  "\U0000f01c", // 21  "wi_sprinkle"
  "\U0000f0b5", // 22  "wi_sleet"
  "\U0000f019", // 23  "wi_rain"
  "\U0000f017", // 24  "wi_rain_mix"
  "\U0000f064", // 25  "wi_snow_wind"
  "\U0000f01b", // 26  "wi_snow"
  "\U0000f01a", // 27  "wi_showers"
  "\U0000f01b", // 28  "wi_snow"
  "\U0000f01e", // 29  "wi_thunderstorm"
  "\U0000f01d", // 30  "wi_storm_showers"
  "\U0000F07B", // 31  "unknown"
};
static char* moon_phase[] ={
  "\U0000F095",//'wi-moon-new':0,
  "\U0000F096",//'wi-moon-waxing-crescent-1',1,
  "\U0000F097",//'wi-moon-waxing-crescent-2',2,
  "\U0000F098",//'wi-moon-waxing-crescent-3',3,
  "\U0000F099",//'wi-moon-waxing-crescent-4',4,
  "\U0000F09A",//'wi-moon-waxing-crescent-5',5,
  "\U0000F09B",//'wi-moon-waxing-crescent-6',6,
  "\U0000F09C",//'wi-moon-first-quarter',7,
  "\U0000F09D",//'wi-moon-waxing-gibbous-1',8,
  "\U0000F09E",//'wi-moon-waxing-gibbous-2',9,
  "\U0000F09F",//'wi-moon-waxing-gibbous-3',10,
  "\U0000F0A0",//'wi-moon-waxing-gibbous-4',11,
  "\U0000F0A1",//'wi-moon-waxing-gibbous-5',12,
  "\U0000F0A2",//'wi-moon-waxing-gibbous-6',13,
  "\U0000F0A3",//'wi-moon-full',14,
  "\U0000F0A4",//'wi-moon-waning-gibbous-1',15,
  "\U0000F0A5",//'wi-moon-waning-gibbous-2',16,
  "\U0000F0A6",//'wi-moon-waning-gibbous-3',17,
  "\U0000F0A7",//'wi-moon-waning-gibbous-4',18,
  "\U0000F0A8",//'wi-moon-waning-gibbous-5',19,
  "\U0000F0A9",//'wi-moon-waning-gibbous-6',20,
  "\U0000F0AA",//'wi-moon-third-quarter',21,
  "\U0000F0AB",//'wi-moon-waning-crescent-1',22,
  "\U0000F0AC",//'wi-moon-waning-crescent-2',23,
  "\U0000F0AD",//'wi-moon-waning-crescent-3',24,
  "\U0000F0AE",//'wi-moon-waning-crescent-4',25,
  "\U0000F0AF",//'wi-moon-waning-crescent-5',26,
  "\U0000F0B0",//'wi-moon-waning-crescent-6',27,
  "\U0000F095",//'wi-moon-new',28,
  "\U0000F07B", // 'unknown': 29,
};

// Save settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Set defaults first
  prv_default_settings();
  // Then override with any saved values
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Apply settings to UI elements
static void prv_update_display() {
  if (settings.NightTheme) {
    if (settings.IsDay) {
      settings.BackgroundColor = settings.BackgroundColorDay;
      settings.TextColor = settings.TextColorDay;
    }
    else {
      settings.BackgroundColor = settings.BackgroundColorNight;
      settings.TextColor = settings.TextColorNight;
    }
  }
  else {
    settings.BackgroundColor = settings.BackgroundColorDay;
    settings.TextColor = settings.TextColorDay;
  }

  // Set background color
  window_set_background_color(s_main_window, settings.BackgroundColor);

  // Set text colors
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
  text_layer_set_text_color(s_date2_layer, settings.TextColor);
  text_layer_set_text_color(s_steps_layer, settings.TextColor);
  text_layer_set_text_color(s_weather_layer, settings.TextColor);
  text_layer_set_text_color(s_weather_icon_layer, settings.TextColor);
  text_layer_set_text_color(s_sunrise_layer, settings.TextColor);
  text_layer_set_text_color(s_sunset_layer, settings.TextColor);
  text_layer_set_text_color(s_moon_layer, settings.TextColor);

  // Show/hide based on setting
  layer_set_hidden(text_layer_get_layer(s_date_layer), !settings.ShowDate);
  layer_set_hidden(text_layer_get_layer(s_date2_layer), (!settings.ShowDate2 || !(PBL_DISPLAY_HEIGHT >= 228)));
  layer_set_hidden(text_layer_get_layer(s_steps_layer), !settings.ShowSteps);
  layer_set_hidden(text_layer_get_layer(s_weather_layer), !settings.ShowWeather);
  layer_set_hidden(text_layer_get_layer(s_weather_icon_layer), !settings.ShowWeather);
  layer_set_hidden(text_layer_get_layer(s_sunrise_layer), !settings.ShowSun);
  layer_set_hidden(text_layer_get_layer(s_sunset_layer), !settings.ShowSun);
  layer_set_hidden(text_layer_get_layer(s_moon_layer), !settings.ShowMoon);
  layer_set_hidden(s_phone_battery_layer, !settings.ShowPhoneBattery);

  // Mark battery layer for redraw (color may have changed)
  layer_mark_dirty(s_battery_layer);
  layer_mark_dirty(s_phone_battery_layer);
}

static void update_time() {
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);

  int time_hours=tick_time->tm_hour;
  int time_minutes=tick_time->tm_min;
  int time_now = time_hours * 100 + time_minutes;
  if (settings.SunriseTime <= time_now && time_now < settings.SunsetTime){
    if (!settings.IsDay) {
      settings.IsDay = true;
      prv_update_display();
    }
  } else{
    if (settings.IsDay) {
      settings.IsDay = false;
      prv_update_display();
    }
  }

  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
                                                    "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_time_buffer);
}

static void update_date() {
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);

  static char s_date_buffer[16];
  static char s_date2_buffer[16];
  if (settings.AltDate) {
    strftime(s_date_buffer, sizeof(s_date_buffer), "%Y-%m-%d", tick_time);
    strftime(s_date2_buffer, sizeof(s_date2_buffer), "%A", tick_time);
  } else {
    strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);
    strftime(s_date2_buffer, sizeof(s_date2_buffer), "%Y", tick_time);
  }
  text_layer_set_text(s_date_layer, s_date_buffer);
  text_layer_set_text(s_date2_layer, s_date2_buffer);
}

static void update_weather() {
  static char temperature_buffer[8];
  if (settings.TemperatureUnit) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°F", settings.WeatherTemp);
  } else {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", settings.WeatherTemp);
  }
  text_layer_set_text(s_weather_layer, temperature_buffer);
  text_layer_set_text(s_weather_icon_layer, weather_conditions[settings.WeatherIcon]);
}

static void update_steps() {
  static char s_steps_buffer[16];
  int step_count = (int)health_service_sum_today(HealthMetricStepCount);
  int thousands = step_count / 1000;
  int hundreds = (step_count % 1000)/100;
  if(thousands > 0) {
    snprintf(s_steps_buffer, sizeof(s_steps_buffer),
      "%d.%d%s", thousands, hundreds, "k");
  } else {
    snprintf(s_steps_buffer, sizeof(s_steps_buffer),
      "%d", step_count);
  }
  text_layer_set_text(s_steps_layer, s_steps_buffer);
}

static void update_sun() {
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  int time_year=tick_time->tm_year;
  int time_month=tick_time->tm_mon + 1;
  int time_day=tick_time->tm_mday;
  time_year = 2026;
  float sunriseTime = calcSunRise(time_year, time_month, time_day, 40.0, -74.0, 90.83f);
  float sunsetTime = calcSunSet(time_year, time_month, time_day, 40.0f, -74.0, 90.83f);
  settings.SunriseTime = (int)(60*(sunriseTime-((int)(sunriseTime)))) + ((int)sunriseTime * 100);
  settings.SunsetTime = (int)(60*(sunsetTime-((int)(sunsetTime)))) + ((int)sunsetTime * 100);
  //todo: convert to local timezone, use gpsLat and gpsLon after converting to float

  static char sunrise_buffer[6];
  static char sunset_buffer[6];
  snprintf(sunrise_buffer, sizeof(sunrise_buffer), "%02d:%02d", (settings.SunriseTime / 100), (settings.SunriseTime % 100));
  snprintf(sunset_buffer, sizeof(sunset_buffer), "%02d:%02d", (settings.SunsetTime / 100), (settings.SunsetTime % 100));
  text_layer_set_text(s_sunrise_layer, sunrise_buffer);
  text_layer_set_text(s_sunset_layer, sunset_buffer);
}

static void update_moon() {
  text_layer_set_text(s_moon_layer, moon_phase[settings.MoonPhase]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // run every minute
  update_time();
  if (settings.ShowSteps){
    update_steps();
  }

  // run every hour
  if (tick_time->tm_min % 60 == 0) {
    update_date();
    if (settings.PeriodicVibrate) {
      vibes_double_pulse();
    }
    // generate message request only if showing info and time matches interval
    if (settings.ShowWeather || settings.ShowSun || settings.ShowMoon || settings.NightTheme) {
      bool requestWeather = false;
      bool requestSun = false;
      // Get weather update every 1-6 hours
      if (settings.ShowWeather && tick_time->tm_hour % settings.WeatherInterval == 0) {
        requestWeather = true;
      }
      // Get sun & moon info every 24 hours
      if ((settings.ShowSun || settings.ShowMoon || settings.NightTheme) && tick_time->tm_hour % 24 == 0) {
        requestSun = true;
      }
      if (requestWeather || requestSun) {
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        if (requestSun) {
          dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUN, 1);
        }
        if (requestWeather) {
          dict_write_uint8(iter, MESSAGE_KEY_REQUEST_WEATHER, 1);
        }
        app_message_outbox_send();
      }
    }
  }
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx, int battery_level) {
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar (inside the border)
  int bar_width = ((battery_level * (bounds.size.w - 4)) / 100);

  // Draw the border using the text color
  graphics_context_set_stroke_color(ctx, settings.TextColor);
  graphics_draw_round_rect(ctx, bounds, 2);

  // Choose color based on battery level
  GColor bar_color;
  if (battery_level <= 20) {
    bar_color = PBL_IF_COLOR_ELSE(GColorRed, settings.TextColor);
  } else if (battery_level <= 40) {
    bar_color = PBL_IF_COLOR_ELSE(GColorChromeYellow, settings.TextColor);
  } else {
    bar_color = PBL_IF_COLOR_ELSE(GColorGreen, settings.TextColor);
  }

  // Draw the filled bar inside the border
  graphics_context_set_fill_color(ctx, bar_color);
  graphics_fill_rect(ctx, GRect(2, 2, bar_width, bounds.size.h - 4), 1, GCornerNone);
}

static void watch_battery_update_proc(Layer *layer, GContext *ctx) {
  battery_update_proc(layer, ctx, s_battery_level);
}

static void phone_battery_update_proc(Layer *layer, GContext *ctx) {
  battery_update_proc(layer, ctx, settings.PhoneBattery);
}

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(text_layer_get_layer(s_moon_layer), !connected || !settings.ShowMoon);
  layer_set_hidden(text_layer_get_layer(s_bt_icon_layer), connected);
  if (!connected) {
    if (settings.BluetoothVibrate) {
      vibes_double_pulse();
    }
  }
}

// AppMessage received handler
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Save previous values to detect actual changes
  bool prev_ShowSun = settings.ShowSun;
  bool prev_ShowMoon = settings.ShowMoon;
  bool prev_NightTheme = settings.NightTheme;
  bool prev_ShowWeather = settings.ShowWeather;
  bool prev_TemperatureUnit = settings.TemperatureUnit;
  bool prev_ShowPhoneBattery = settings.ShowPhoneBattery;
  bool prev_AltDate = settings.AltDate;

  // Check for Clay settings data
  Tuple *bg_color_day_t = dict_find(iterator, MESSAGE_KEY_BackgroundColorDay);
  if (bg_color_day_t) {
    settings.BackgroundColorDay = GColorFromHEX(bg_color_day_t->value->int32);
  }
  Tuple *text_color_day_t = dict_find(iterator, MESSAGE_KEY_TextColorDay);
  if (text_color_day_t) {
    settings.TextColorDay = GColorFromHEX(text_color_day_t->value->int32);
  }
  Tuple *bg_color_night_t = dict_find(iterator, MESSAGE_KEY_BackgroundColorNight);
  if (bg_color_night_t) {
    settings.BackgroundColorNight = GColorFromHEX(bg_color_night_t->value->int32);
  }
  Tuple *text_color_night_t = dict_find(iterator, MESSAGE_KEY_TextColorNight);
  if (text_color_night_t) {
    settings.TextColorNight = GColorFromHEX(text_color_night_t->value->int32);
  }
  Tuple *night_theme_t = dict_find(iterator, MESSAGE_KEY_NightTheme);
  if (night_theme_t) {
    settings.NightTheme = night_theme_t->value->int32 == 1;
  }
  Tuple *show_date_t = dict_find(iterator, MESSAGE_KEY_ShowDate);
  if (show_date_t) {
    settings.ShowDate = show_date_t->value->int32 == 1;
  }
  Tuple *show_date2_t = dict_find(iterator, MESSAGE_KEY_ShowDate2);
  if (show_date2_t) {
    settings.ShowDate2 = show_date2_t->value->int32 == 1;
  }
  Tuple *alt_date_t = dict_find(iterator, MESSAGE_KEY_AltDate);
  if (alt_date_t) {
    settings.AltDate = alt_date_t->value->int32 == 1;
  }
  Tuple *show_weather_t = dict_find(iterator, MESSAGE_KEY_ShowWeather);
  if (show_weather_t) {
    settings.ShowWeather = show_weather_t->value->int32 == 1;
  }
  Tuple *temp_unit_t = dict_find(iterator, MESSAGE_KEY_TemperatureUnit);
  if (temp_unit_t) {
    settings.TemperatureUnit = temp_unit_t->value->int32 == 1;
  }
  Tuple *weahter_interval_t = dict_find(iterator, MESSAGE_KEY_WeatherInterval);
  if (weahter_interval_t) {
    settings.WeatherInterval = (int)weahter_interval_t->value->int32;
  }
  Tuple *show_steps_t = dict_find(iterator, MESSAGE_KEY_ShowSteps);
  if (show_steps_t) {
    settings.ShowSteps = show_steps_t->value->int32 == 1;
    update_steps();
  }
  Tuple *show_sun_t = dict_find(iterator, MESSAGE_KEY_ShowSun);
  if (show_sun_t) {
    settings.ShowSun = show_sun_t->value->int32 == 1;
  }
  Tuple *show_moon_t = dict_find(iterator, MESSAGE_KEY_ShowMoon);
  if (show_moon_t) {
    settings.ShowMoon = show_moon_t->value->int32 == 1;
  }
  Tuple *show_phone_battery_t = dict_find(iterator, MESSAGE_KEY_ShowPhoneBattery);
  if (show_phone_battery_t) {
    settings.ShowPhoneBattery = show_phone_battery_t->value->int32 == 1;
  }
  Tuple *periodic_vibrate_t = dict_find(iterator, MESSAGE_KEY_PeriodicVibrate);
  if (periodic_vibrate_t) {
    settings.PeriodicVibrate = periodic_vibrate_t->value->int32 == 1;
  }
  Tuple *disconnect_alert_t = dict_find(iterator, MESSAGE_KEY_BluetoothVibrate);
  if (disconnect_alert_t) {
    settings.BluetoothVibrate = disconnect_alert_t->value->int32 == 1;
  }


  Tuple *gps_lat_t = dict_find(iterator, MESSAGE_KEY_GpsLat);
  if (gps_lat_t) {    
    snprintf(settings.GpsLat, sizeof(settings.GpsLat), "%s", gps_lat_t->value->cstring);
  }
  Tuple *gps_lon_t = dict_find(iterator, MESSAGE_KEY_GpsLon);
  if (gps_lon_t) {    
    snprintf(settings.GpsLon, sizeof(settings.GpsLat), "%s", gps_lon_t->value->cstring);
  }

  // Check for weather data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

  if (temp_tuple && conditions_tuple) {
    settings.WeatherTemp = (int)temp_tuple->value->int32;
    // Convert to Fahrenheit if setting is enabled
    if (settings.TemperatureUnit) {
      settings.WeatherTemp = (settings.WeatherTemp * 9 / 5) + 32;
    }
    settings.WeatherIcon = (int)conditions_tuple->value->int32;
  }
  if (show_weather_t || (temp_tuple && conditions_tuple)) {
    update_weather();
  }

  // Check for sun info
  Tuple *sunrise_tuple = dict_find(iterator, MESSAGE_KEY_SUNRISE);
  Tuple *sunset_tuple = dict_find(iterator, MESSAGE_KEY_SUNSET);
  if (sunrise_tuple && sunset_tuple) {
    settings.SunriseTime = (int)sunrise_tuple->value->int32;
    settings.SunsetTime = (int)sunset_tuple->value->int32;
  }
  if (show_sun_t || (sunrise_tuple && sunset_tuple)) {
    update_sun();
  }
  Tuple *moon_tuple = dict_find(iterator, MESSAGE_KEY_MOONPHASE);
  if (moon_tuple) {
    settings.MoonPhase = (int)moon_tuple->value->int32;
  }
  if (show_moon_t || moon_tuple) {
    update_moon();
  }

  // check for battery data
  Tuple *battery_tuple = dict_find(iterator, MESSAGE_KEY_BATTERY);
  if (battery_tuple) {
    settings.PhoneBattery = (int)battery_tuple->value->int32;
    layer_mark_dirty(s_phone_battery_layer);
  }

  // Save and apply if any settings were changed
  if (bg_color_day_t || text_color_day_t || bg_color_night_t || text_color_night_t || night_theme_t || temp_unit_t || show_weather_t || show_date_t || show_date2_t || alt_date_t || show_steps_t || show_sun_t || show_moon_t || show_phone_battery_t) {
    
    // if show battery was toggled
    if (prev_ShowPhoneBattery != settings.ShowPhoneBattery) {
      int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
      int bar_height = (PBL_DISPLAY_HEIGHT / 24);
      int bar_width = PBL_DISPLAY_WIDTH / 1.1;
      int bar_x = (PBL_DISPLAY_WIDTH - bar_width) / 2;
      int bar_y = PBL_DISPLAY_HEIGHT - (bar_offset - (PBL_DISPLAY_HEIGHT / 12));
      if (settings.ShowPhoneBattery) {
        bar_width = (bar_width / 2) - (bar_x / 2);
      }
      layer_set_frame(s_battery_layer, GRect(bar_x, bar_y, bar_width, bar_height));
      layer_mark_dirty(s_battery_layer);
    }
    
    prv_save_settings();
    prv_update_display();

    // Request data when a setting was changed
    bool requestSun = (!prev_ShowSun && settings.ShowSun) ||
                   (!prev_ShowMoon && settings.ShowMoon) ||
                   (!prev_NightTheme && settings.NightTheme);
    bool requestWeather = ((prev_TemperatureUnit != settings.TemperatureUnit) || !prev_ShowWeather) && settings.ShowWeather;
    bool requestBattery = (!prev_ShowPhoneBattery && settings.ShowPhoneBattery);
    bool unsibscribeBattery = (prev_ShowPhoneBattery && !settings.ShowPhoneBattery);
    if (requestSun || requestWeather || requestBattery || unsibscribeBattery) {
      DictionaryIterator *iter;
      app_message_outbox_begin(&iter);
      if (requestSun) {
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUN, 1);
      }
      if (requestWeather) {
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_WEATHER, 1);
      }
      if (requestBattery) {
        dict_write_uint8(iter, MESSAGE_KEY_REQUEST_BATTERY, 1);
      }
      if (unsibscribeBattery) {
        dict_write_uint8(iter, MESSAGE_KEY_UNSUBSCRIBE_BATTERY, 1);
      }
      app_message_outbox_send();
    }

    // update if date format changes
    if (prev_AltDate != settings.AltDate) {
      update_date();
    }
  } else if (temp_tuple || conditions_tuple || sunrise_tuple || sunset_tuple || moon_tuple || battery_tuple) {
    prv_save_settings();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

// Unobstructed area handlers
static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  // Hide layers during the transition to reduce clutter
  layer_set_hidden(text_layer_get_layer(s_bt_icon_layer), true);
  layer_set_hidden(text_layer_get_layer(s_steps_layer), true);
  layer_set_hidden(text_layer_get_layer(s_weather_layer), true);
  layer_set_hidden(text_layer_get_layer(s_weather_icon_layer), true);
  layer_set_hidden(text_layer_get_layer(s_sunrise_layer), true);
  layer_set_hidden(text_layer_get_layer(s_sunset_layer), true);
  layer_set_hidden(text_layer_get_layer(s_moon_layer), true);
}

static void prv_unobstructed_change(AnimationProgress progress, void *context) {
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);

  // Reposition layers to fit in the available space
  int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
  int bar_y = bounds.size.h - (bar_offset - (bounds.size.h / 12));

  GRect watch_battery_frame = layer_get_frame(s_battery_layer);
  watch_battery_frame.origin.y = bar_y;
  layer_set_frame(s_battery_layer, watch_battery_frame);

  GRect phone_battery_frame = layer_get_frame(s_phone_battery_layer);
  phone_battery_frame.origin.y = bar_y;
  layer_set_frame(s_phone_battery_layer, phone_battery_frame);
}

static void prv_unobstructed_did_change(void *context) {
  GRect full_bounds = layer_get_bounds(s_window_layer);
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);
  bool obstructed = !grect_equal(&full_bounds, &bounds);

  // Keep layers hidden when obstructed, otherwise restore based on setting or connection
  if (obstructed) {
    layer_set_hidden(text_layer_get_layer(s_bt_icon_layer), true);
    layer_set_hidden(text_layer_get_layer(s_steps_layer), true);
    layer_set_hidden(text_layer_get_layer(s_weather_layer), true);
    layer_set_hidden(text_layer_get_layer(s_weather_icon_layer), true);
    layer_set_hidden(text_layer_get_layer(s_sunrise_layer), true);
    layer_set_hidden(text_layer_get_layer(s_sunset_layer), true);
    layer_set_hidden(text_layer_get_layer(s_moon_layer), true);
  } else {
    layer_set_hidden(text_layer_get_layer(s_bt_icon_layer),
      connection_service_peek_pebble_app_connection());
    layer_set_hidden(text_layer_get_layer(s_steps_layer), !settings.ShowSteps);
    layer_set_hidden(text_layer_get_layer(s_weather_layer), !settings.ShowWeather);
    layer_set_hidden(text_layer_get_layer(s_weather_icon_layer), !settings.ShowWeather);
    layer_set_hidden(text_layer_get_layer(s_sunrise_layer), !settings.ShowSun);
    layer_set_hidden(text_layer_get_layer(s_sunset_layer), !settings.ShowSun);
    layer_set_hidden(text_layer_get_layer(s_moon_layer), (!settings.ShowMoon || !connection_service_peek_pebble_app_connection()));
  }
}

static void main_window_load(Window *window) {
  s_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(s_window_layer);

  // set custom font
  int info_padding;
  int info_height;
  int time_padding;
  int time_height;
  int date_padding;
  int date_height;
  s_info_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  info_padding = 10;
  info_height = 28;
  s_date_font = s_info_font;
  date_padding = info_padding;
  date_height = info_height;
  s_bt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_18));
  #if defined(PBL_PLATFORM_EMERY)
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TALLBOLD_64));
    time_padding = 2;
    time_height = 64;
  #else
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TALLBOLD_49));
    time_padding = 2;
    time_height = 49;
  #endif

  // Position the time + date block
  int date2_y = (bounds.size.h / 16) - date_padding;
  int date_y = date2_y;
  if (PBL_DISPLAY_HEIGHT >= 228) {
    date_y = date_y + date_height;
  }
  int time_y = date_y + date_height + date_padding - time_padding;

  // Create the time TextLayer
  s_time_layer = text_layer_create(
      GRect(0, time_y, bounds.size.w, time_height));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create the date TextLayer
  s_date_layer = text_layer_create(
      GRect(0, date_y, bounds.size.w, (info_height + 4)));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
  text_layer_set_font(s_date_layer, s_info_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  s_date2_layer = text_layer_create(
      GRect(0, date2_y, bounds.size.w, (info_height + 4)));
  text_layer_set_background_color(s_date2_layer, GColorClear);
  text_layer_set_text_color(s_date2_layer, settings.TextColor);
  text_layer_set_font(s_date2_layer, s_info_font);
  text_layer_set_text_alignment(s_date2_layer, GTextAlignmentCenter);

  // Create battery meter Layer
  int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
  int bar_height = (PBL_DISPLAY_HEIGHT / 24);
  int bar_width = bounds.size.w / 1.1;
  int bar_x = (bounds.size.w - bar_width) / 2;
  int bar_y = bounds.size.h - (bar_offset - (bounds.size.h / 12));
  int phone_bar_width = (bar_width / 2) - (bar_x / 2);
  if (settings.ShowPhoneBattery) {
    bar_width = phone_bar_width;
  }
  s_battery_layer = layer_create(GRect(bar_x, bar_y, bar_width, bar_height));
  layer_set_update_proc(s_battery_layer, watch_battery_update_proc);

  // Create phone battery meter Layer
  int phone_bar_y = bar_y;
  int phone_bar_x = (bar_x * 2) + phone_bar_width;
  s_phone_battery_layer = layer_create(GRect(phone_bar_x, phone_bar_y, phone_bar_width, bar_height));
  layer_set_update_proc(s_phone_battery_layer, phone_battery_update_proc);

  // Create weather TextLayer
  int weather_y = bar_y - (info_height * 2) - (bounds.size.h / 15);
  s_weather_layer = text_layer_create(
      GRect(0, weather_y, ((bounds.size.w / 10) * 4), (info_height + 4)));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, settings.TextColor);
  text_layer_set_font(s_weather_layer, s_info_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentRight);

  // Create weather icon TextLayer
  int weather_icon_y = weather_y + (info_padding * 0.75);
  s_weather_icon_layer = text_layer_create(
      GRect(((bounds.size.w / 10) * 4), weather_icon_y, ((bounds.size.w / 10) * 2), (info_height + 4)));
  text_layer_set_background_color(s_weather_icon_layer, GColorClear);
  text_layer_set_text_color(s_weather_icon_layer, settings.TextColor);
  text_layer_set_font(s_weather_icon_layer, s_weather_font);
  text_layer_set_text_alignment(s_weather_icon_layer, GTextAlignmentRight);

  // Create steps TextLayer
  int steps_y = weather_y;
  s_steps_layer = text_layer_create(
      GRect(((bounds.size.w / 10) * 6), steps_y, ((bounds.size.w / 10) * 4), (info_height + 4)));
  text_layer_set_background_color(s_steps_layer, GColorClear);
  text_layer_set_text_color(s_steps_layer, settings.TextColor);
  text_layer_set_font(s_steps_layer, s_info_font);
  text_layer_set_text_alignment(s_steps_layer, GTextAlignmentCenter);

  // Create sun TextLayer
  int sun_y = weather_y + info_height;
  s_sunrise_layer = text_layer_create(
      GRect(0, sun_y, ((bounds.size.w / 5) * 2), (info_height + 4)));
  text_layer_set_background_color(s_sunrise_layer, GColorClear);
  text_layer_set_text_color(s_sunrise_layer, settings.TextColor);
  text_layer_set_font(s_sunrise_layer, s_info_font);
  text_layer_set_text_alignment(s_sunrise_layer, GTextAlignmentRight);
  s_sunset_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 3), sun_y, ((bounds.size.w / 5) * 2), (info_height + 4)));
  text_layer_set_background_color(s_sunset_layer, GColorClear);
  text_layer_set_text_color(s_sunset_layer, settings.TextColor);
  text_layer_set_font(s_sunset_layer, s_info_font);
  text_layer_set_text_alignment(s_sunset_layer, GTextAlignmentLeft);

  // Create the moon layer
  int moon_y = sun_y + (info_padding * 0.85);
  //moon_y = 0;
  s_moon_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 2), moon_y, ((bounds.size.w / 5) * 1), (info_height + 4)));
  text_layer_set_background_color(s_moon_layer, GColorClear);
  text_layer_set_text_color(s_moon_layer, settings.TextColor);
  text_layer_set_font(s_moon_layer, s_weather_font);
  text_layer_set_text_alignment(s_moon_layer, GTextAlignmentCenter);

  // Create the Bluetooth icon
  int bt_y = sun_y + (info_padding * 1.1);
  s_bt_icon_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 2), bt_y, ((bounds.size.w / 5) * 1), (info_height + 4)));
  text_layer_set_background_color(s_bt_icon_layer, GColorClear);
  text_layer_set_text_color(s_bt_icon_layer, settings.TextColor);
  text_layer_set_font(s_bt_icon_layer, s_bt_font);
  text_layer_set_text_alignment(s_bt_icon_layer, GTextAlignmentCenter);
  text_layer_set_text(s_bt_icon_layer, "z");

  // Add layers to the Window
  layer_add_child(s_window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_date2_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_weather_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_weather_icon_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_steps_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_sunrise_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_sunset_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_moon_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_bt_icon_layer));
  layer_add_child(s_window_layer, s_battery_layer);
  layer_add_child(s_window_layer, s_phone_battery_layer);

  // Apply saved settings
  prv_update_display();

  // Apply correct layout in case Quick View is already active
  prv_unobstructed_change(0, NULL);
  prv_unobstructed_did_change(NULL);

  // Subscribe to unobstructed area events
  UnobstructedAreaHandlers handlers = {
    .will_change = prv_unobstructed_will_change,
    .change = prv_unobstructed_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe(handlers, NULL);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_date2_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_weather_icon_layer);
  text_layer_destroy(s_steps_layer);
  text_layer_destroy(s_sunrise_layer);
  text_layer_destroy(s_sunset_layer);
  text_layer_destroy(s_moon_layer);
  text_layer_destroy(s_bt_icon_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_bt_font);
  fonts_unload_custom_font(s_weather_font);
  layer_destroy(s_battery_layer);
  layer_destroy(s_phone_battery_layer);
}

static void init() {
  // Load settings before creating UI
  prv_load_settings();

  s_main_window = window_create();
  window_set_background_color(s_main_window, settings.BackgroundColor);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  // set initial values
  update_time();
  update_date();
  if (settings.ShowWeather){
    update_weather();
  }
  if (settings.ShowSteps){
    update_steps();
  }
  if (settings.ShowSun){
    update_sun();
  }
  if (settings.ShowMoon){
    update_moon();
  }
  battery_callback(battery_state_service_peek());

  // subscribe to events
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_callback);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  // Register AppMessage callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  const int inbox_size = 256;
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
