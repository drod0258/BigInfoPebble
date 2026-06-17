#include <pebble.h>

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  // user settings
  int BacklightColor;
  GColor BackgroundColor;
  GColor TimeColor;
  GColor DateColor;
  GColor WeatherColor;
  GColor HealthColor;
  GColor SunColor;
  GColor MoonColor;
  GColor BatteryOutlineColor;
  GColor BatteryChargingColor;
  GColor BatteryFullColor;
  GColor BatteryMidColor;
  GColor BatteryLowColor;
  int BatteryMidPercent;
  int BatteryLowPercent;
  bool ShowCharging;
  int BacklightColorDay;
  GColor BackgroundColorDay;
  GColor TextColorDay;
  GColor TimeColorDay;
  GColor DateColorDay;
  GColor WeatherColorDay;
  GColor HealthColorDay;
  GColor SunColorDay;
  GColor MoonColorDay;
  GColor BatteryOutlineColorDay;
  GColor BatteryChargingColorDay;
  GColor BatteryFullColorDay;
  GColor BatteryMidColorDay;
  GColor BatteryLowColorDay;
  int BacklightColorNight;
  GColor BackgroundColorNight;
  GColor TextColorNight;
  GColor TimeColorNight;
  GColor DateColorNight;
  GColor WeatherColorNight;
  GColor HealthColorNight;
  GColor SunColorNight;
  GColor MoonColorNight;
  GColor BatteryOutlineColorNight;
  GColor BatteryChargingColorNight;
  GColor BatteryFullColorNight;
  GColor BatteryMidColorNight;
  GColor BatteryLowColorNight;
  bool NightTheme;
  bool ShowWeather;
  bool TemperatureUnit;
  int WeatherInterval;
  bool ShowDate;
  bool ShowDate2;
  bool AltDate;
  bool ShowSteps;
  bool ShowHR;
  bool ShowSun;
  bool ShowMoon;
  bool ShowPhoneBattery;
  bool PeriodicVibrate;
  bool PeriodicSound;
  bool BluetoothVibrate;
  bool BluetoothSound;
  int Volume;
  int Latitude;
  int Longitude;
  // storage
  bool IsDay;
  bool ManualCoordinates;
  int SunriseTime;
  int SunsetTime;
  int MoonPhase;
  int WeatherTemp;
  int WeatherIcon;
  int PhoneBattery;
  bool PhoneCharging;
  bool requestSun;
  bool requestWeather;
  bool requestBattery;
  bool unsibscribeBattery;
  bool updateCoordinates;
  bool requestJS;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;
static bool s_js_ready;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_date2_layer;
static TextLayer *s_health_layer;
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
static bool s_battery_charging;
static Layer *s_phone_battery_layer;

// Unobstructed area
static Layer *s_window_layer;

// Set default settings
static void prv_default_settings() {
  // user settings
  settings.BacklightColorDay = 0xFFAA55;
  settings.BackgroundColorDay = GColorWhite;
  settings.TextColorDay = GColorBlack;
  settings.TimeColorDay = GColorBlack;
  settings.DateColorDay = GColorBlack;
  settings.WeatherColorDay = GColorBlack;
  settings.HealthColorDay = GColorBlack;
  settings.SunColorDay = GColorBlack;
  settings.MoonColorDay = GColorBlack;
  settings.BatteryOutlineColorDay = GColorBlack;
  settings.BatteryChargingColorDay = GColorBlue;
  settings.BatteryFullColorDay = GColorGreen;
  settings.BatteryMidColorDay = GColorChromeYellow;
  settings.BatteryLowColorDay = GColorRed;
  settings.BacklightColorNight = 0xFF5500;
  settings.BackgroundColorNight = GColorBlack;
  settings.TextColorNight = GColorWhite;
  settings.TimeColorNight = GColorWhite;
  settings.DateColorNight = GColorWhite;
  settings.WeatherColorNight = GColorWhite;
  settings.HealthColorNight = GColorWhite;
  settings.SunColorNight = GColorWhite;
  settings.MoonColorNight = GColorWhite;
  settings.BatteryOutlineColorNight = GColorWhite;
  settings.BatteryChargingColorNight = GColorBlue;
  settings.BatteryFullColorNight = GColorGreen;
  settings.BatteryMidColorNight = GColorChromeYellow;
  settings.BatteryLowColorNight = GColorRed;
  settings.BacklightColor = settings.BacklightColorDay;
  settings.BackgroundColor = settings.BackgroundColorDay;
  settings.TimeColor = settings.TimeColorDay;
  settings.DateColor = settings.DateColorDay;
  settings.WeatherColor = settings.WeatherColorDay;
  settings.HealthColor = settings.HealthColorDay;
  settings.SunColor = settings.SunColorDay;
  settings.MoonColor = settings.MoonColorDay;
  settings.BatteryOutlineColor = settings.BatteryOutlineColorDay;
  settings.BatteryChargingColor = settings.BatteryChargingColorDay;
  settings.BatteryFullColor = settings.BatteryFullColorDay;
  settings.BatteryMidColor = settings.BatteryMidColorDay;
  settings.BatteryLowColor = settings.BatteryLowColorDay;
  settings.BatteryMidPercent = 40;
  settings.BatteryLowPercent = 20;
  settings.ShowCharging = false;
  settings.NightTheme = false;
  settings.ShowDate = false;
  settings.ShowDate2 = false;
  settings.AltDate = false;
  settings.ShowWeather = false;
  settings.TemperatureUnit = false;
  settings.WeatherInterval = 3;
  settings.ShowSteps = false;
  settings.ShowHR = false;
  settings.ShowSun = false;
  settings.ShowMoon = false;
  settings.ShowPhoneBattery = false;
  settings.PeriodicVibrate = false;
  settings.PeriodicSound = false;
  settings.BluetoothVibrate = false;
  settings.BluetoothSound = false;
  settings.Volume = 50;
  // storage
  settings.IsDay=false;
  settings.ManualCoordinates=false;
  settings.SunriseTime=1;
  settings.SunsetTime=2359;
  settings.MoonPhase=29;
  settings.WeatherTemp=-99;
  settings.WeatherIcon=15;
  settings.PhoneBattery=0;
  settings.PhoneCharging=false;
  settings.requestSun=false;
  settings.requestWeather=false;
  settings.requestBattery=false;
  settings.unsibscribeBattery=false;
  settings.updateCoordinates=false;
  settings.requestJS=false;
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
  if (settings.NightTheme && !settings.IsDay) {
    settings.BacklightColor = settings.BacklightColorNight;
    settings.BackgroundColor = settings.BackgroundColorNight;
    settings.TimeColor = PBL_IF_COLOR_ELSE(settings.TimeColorNight, settings.TextColorNight);
    settings.DateColor = PBL_IF_COLOR_ELSE(settings.DateColorNight, settings.TextColorNight);
    settings.WeatherColor = PBL_IF_COLOR_ELSE(settings.WeatherColorNight, settings.TextColorNight);
    settings.HealthColor = PBL_IF_COLOR_ELSE(settings.HealthColorNight, settings.TextColorNight);
    settings.SunColor = PBL_IF_COLOR_ELSE(settings.SunColorNight, settings.TextColorNight);
    settings.MoonColor = PBL_IF_COLOR_ELSE(settings.MoonColorNight, settings.TextColorNight);
    settings.BatteryOutlineColor = PBL_IF_COLOR_ELSE(settings.BatteryOutlineColorNight, settings.TextColorNight);
    settings.BatteryChargingColor = PBL_IF_COLOR_ELSE(settings.BatteryChargingColorNight, settings.TextColorNight);
    settings.BatteryFullColor = PBL_IF_COLOR_ELSE(settings.BatteryFullColorNight, settings.TextColorNight);
    settings.BatteryMidColor = PBL_IF_COLOR_ELSE(settings.BatteryMidColorNight, settings.TextColorNight);
    settings.BatteryLowColor = PBL_IF_COLOR_ELSE(settings.BatteryLowColorNight, settings.TextColorNight);
  }
  else {
    settings.BacklightColor = settings.BacklightColorDay;
    settings.BackgroundColor = settings.BackgroundColorDay;
    settings.TimeColor = PBL_IF_COLOR_ELSE(settings.TimeColorDay, settings.TextColorDay);
    settings.DateColor = PBL_IF_COLOR_ELSE(settings.DateColorDay, settings.TextColorDay);
    settings.WeatherColor = PBL_IF_COLOR_ELSE(settings.WeatherColorDay, settings.TextColorDay);
    settings.HealthColor = PBL_IF_COLOR_ELSE(settings.HealthColorDay, settings.TextColorDay);
    settings.SunColor = PBL_IF_COLOR_ELSE(settings.SunColorDay, settings.TextColorDay);
    settings.MoonColor = PBL_IF_COLOR_ELSE(settings.MoonColorDay, settings.TextColorDay);
    settings.BatteryOutlineColor = PBL_IF_COLOR_ELSE(settings.BatteryOutlineColorDay, settings.TextColorDay);
    settings.BatteryChargingColor = PBL_IF_COLOR_ELSE(settings.BatteryChargingColorDay, settings.TextColorDay);
    settings.BatteryFullColor = PBL_IF_COLOR_ELSE(settings.BatteryFullColorDay, settings.TextColorDay);
    settings.BatteryMidColor = PBL_IF_COLOR_ELSE(settings.BatteryMidColorDay, settings.TextColorDay);
    settings.BatteryLowColor = PBL_IF_COLOR_ELSE(settings.BatteryLowColorDay, settings.TextColorDay);
  }

  #if defined(PBL_RGB_BACKLIGHT)
    // If color is set to black use system default color, else set the backlight color
    if (settings.BacklightColor == 0x000000) {
      light_set_system_color();
    } else {
      light_set_color_rgb888(settings.BacklightColor);
    }
  #endif

  // Set background color
  window_set_background_color(s_main_window, settings.BackgroundColor);

  // Set text colors
  text_layer_set_text_color(s_time_layer, settings.TimeColor);
  text_layer_set_text_color(s_date_layer, settings.DateColor);
  text_layer_set_text_color(s_date2_layer, settings.DateColor);
  text_layer_set_text_color(s_health_layer, settings.HealthColor);
  text_layer_set_text_color(s_weather_layer, settings.WeatherColor);
  text_layer_set_text_color(s_weather_icon_layer, settings.WeatherColor);
  text_layer_set_text_color(s_sunrise_layer, settings.SunColor);
  text_layer_set_text_color(s_sunset_layer, settings.SunColor);
  text_layer_set_text_color(s_moon_layer, settings.MoonColor);
  text_layer_set_text_color(s_bt_icon_layer, settings.TimeColor);

  // Show/hide based on setting
  layer_set_hidden(text_layer_get_layer(s_date_layer), !settings.ShowDate);
  layer_set_hidden(text_layer_get_layer(s_date2_layer), (!settings.ShowDate2 || !(PBL_DISPLAY_HEIGHT >= 228)));
  layer_set_hidden(text_layer_get_layer(s_health_layer), (!settings.ShowSteps && !settings.ShowHR));
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
                                                    "%H:%M" : "%l:%M", tick_time);
  // "%l" blank-pads single-digit hours (e.g. " 9:00"). With center alignment the
  // leading space gets counted, pushing the visible digits off-center. Skip it so
  // the time stays centered whether the hour has one or two digits.
  char *time_text = s_time_buffer;
  if (time_text[0] == ' ') {
    time_text++;
  }
  text_layer_set_text(s_time_layer, time_text);
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

static void update_health() {
  static char s_steps_buffer[12];
  static char s_hr_buffer[8];
  static char s_space_buffer[4];
  static char s_health_buffer[24];
  if (settings.ShowSteps) {
    int step_count = (int)health_service_sum_today(HealthMetricStepCount);
    int thousands = step_count / 1000;
    int hundreds = (step_count % 1000)/100;
    if(thousands > 0) {
      snprintf(s_steps_buffer, sizeof(s_steps_buffer), "%d.%d%s", thousands, hundreds, "k");
    } else {
      snprintf(s_steps_buffer, sizeof(s_steps_buffer), "%d", step_count);
    }
    if (settings.ShowHR && (PBL_DISPLAY_HEIGHT >= 228)) {
      if (thousands >= 10) {
        snprintf(s_space_buffer, sizeof(s_space_buffer), "%s", "  ");
      } else {
        snprintf(s_space_buffer, sizeof(s_space_buffer), "%s", "   ");
      }
    } else {
      snprintf(s_space_buffer, sizeof(s_space_buffer), "%s", "");
    }
  } else {
    snprintf(s_steps_buffer, sizeof(s_steps_buffer), "%s", "");
    snprintf(s_space_buffer, sizeof(s_space_buffer), "%s", "");
  }
  // only show HR on small screens if steps are not shown
  if (settings.ShowHR && (!settings.ShowSteps || PBL_DISPLAY_HEIGHT >= 228)) {
    int hr = (int)health_service_peek_current_value(HealthMetricHeartRateBPM);
    snprintf(s_hr_buffer, sizeof(s_hr_buffer), "%d", hr);
  } else {
    snprintf(s_hr_buffer, sizeof(s_hr_buffer), "%s", "");
  }
  snprintf(s_health_buffer, sizeof(s_health_buffer), "%s%s%s", s_steps_buffer, s_space_buffer, s_hr_buffer);
  text_layer_set_text(s_health_layer, s_health_buffer);
}

static void update_sun() {
  static char sunrise_buffer[6];
  static char sunset_buffer[6];
  int sunriseHour = (settings.SunriseTime / 100);
  int sunriseMinute = (settings.SunriseTime % 100);
  int sunsetHour = (settings.SunsetTime / 100);
  int sunsetMinute = (settings.SunsetTime % 100);
  if (!clock_is_24h_style()) {
    if (sunriseHour > 12) {
      sunriseHour -= 12;
    } else if (sunriseHour == 0) {
      sunriseHour = 12;
    }
    if (sunsetHour > 12) {
      sunsetHour -= 12;
    } else if (sunsetHour == 0) {
      sunsetHour = 12;
    }
  }
  snprintf(sunrise_buffer, sizeof(sunrise_buffer), clock_is_24h_style() ? "%02d:%02d" : "%d:%02d", sunriseHour, sunriseMinute);
  snprintf(sunset_buffer, sizeof(sunset_buffer), clock_is_24h_style() ? "%02d:%02d" : "%d:%02d", sunsetHour, sunsetMinute);
  text_layer_set_text(s_sunrise_layer, sunrise_buffer);
  text_layer_set_text(s_sunset_layer, sunset_buffer);
}

static void update_moon() {
  text_layer_set_text(s_moon_layer, moon_phase[settings.MoonPhase]);
}

int parse_coordinates(char *coor_str) {
  char *dot = strchr(coor_str, '.');
  int whole_part = 0;
  int frac_part = 0;
  
  if (dot) {
    // Null-terminate at the dot to isolate the whole number
    *dot = '\0';
    whole_part = atoi(coor_str);
    
    // Move to the character after the dot
    char *fractional_string = dot + 1;
    int len = strlen(fractional_string);
    frac_part = atoi(fractional_string);
    
    // Scale the fractional part to 6 decimal places
    // e.g., if ".5" it becomes 500000. If ".5074" it becomes 507400
    for (int i = len; i < 6; i++) {
      frac_part *= 10;
    }
    // If the fractional part was longer than 6, truncate it
    for (int i = len; i > 6; i--) {
      frac_part /= 10;
    }
  } else {
    // No decimal point found
    whole_part = atoi(coor_str);
  }

  // Combine them (handling negative coordinates like -74.00)
  if (whole_part < 0 || coor_str[0] == '-') {
    return (whole_part * 1000000) - frac_part;
  } else {
    return (whole_part * 1000000) + frac_part;
  }
}

static void request_js() {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (settings.requestSun) {
    dict_write_uint8(iter, MESSAGE_KEY_REQUEST_SUN, 1);
    settings.requestSun = false;
  }
  if (settings.requestWeather) {
    dict_write_uint8(iter, MESSAGE_KEY_REQUEST_WEATHER, 1);
    settings.requestWeather = false;
  }
  if (settings.requestBattery) {
    dict_write_uint8(iter, MESSAGE_KEY_REQUEST_BATTERY, 1);
    settings.requestBattery = false;
  }
  if (settings.unsibscribeBattery) {
    dict_write_uint8(iter, MESSAGE_KEY_UNSUBSCRIBE_BATTERY, 1);
    settings.unsibscribeBattery = false;
  }
  if (settings.updateCoordinates) {
    // Send scaled int if coordinates are set, otherwise send empty string
    if (settings.ManualCoordinates) {
      dict_write_int32(iter, MESSAGE_KEY_Latitude, settings.Latitude);
      dict_write_int32(iter, MESSAGE_KEY_Longitude, settings.Longitude);
    } else {
      dict_write_cstring(iter, MESSAGE_KEY_Latitude, "");
      dict_write_cstring(iter, MESSAGE_KEY_Longitude, "");
    }
    settings.updateCoordinates = false;
  }
  settings.requestJS=false;
  app_message_outbox_send();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // run every minute
  update_time();
  if (settings.ShowSteps || settings.ShowHR){
    update_health();
  }

  // run every hour
  if (tick_time->tm_min % 60 == 0) {
    update_date();
    if (settings.PeriodicVibrate) {
      vibes_double_pulse();
    }
    #if defined(PBL_SPEAKER)
      if (settings.PeriodicSound) {
        static const SpeakerNote s_single_beep_sine[] = {
          { .midi_note = 108, .waveform = SpeakerWaveformSine,  .duration_ms = 150 },
          { .midi_note = 0,   .waveform = SpeakerWaveformSine,  .duration_ms = 100 }
        };
        speaker_play_notes(s_single_beep_sine, ARRAY_LENGTH(s_single_beep_sine), settings.Volume);
      }
    #endif
    // generate message request only if showing info and time matches interval
    if (settings.ShowWeather || settings.ShowSun || settings.ShowMoon || settings.NightTheme) {
      // Get weather update every 1-6 hours
      if (settings.ShowWeather && tick_time->tm_hour % settings.WeatherInterval == 0) {
        settings.requestWeather = true;
      }
      // Get sun & moon info every 24 hours
      if ((settings.ShowSun || settings.ShowMoon || settings.NightTheme) && tick_time->tm_hour % 24 == 0) {
        settings.requestSun = true;
      }
      if (settings.requestWeather || settings.requestSun) {
        // request data only if currently connected to phone
        if (connection_service_peek_pebble_app_connection()) {
          request_js();
        } else {
          settings.requestJS=true;
          prv_save_settings();
        }
      }
    }
  }
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  s_battery_charging = state.is_charging;
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx, int battery_level, bool is_charging) {
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar (inside the border)
  int bar_width = ((battery_level * (bounds.size.w - 4)) / 100);

  // Draw the border using the text color
  graphics_context_set_stroke_color(ctx, settings.BatteryOutlineColor);
  graphics_draw_round_rect(ctx, bounds, 2);

  // Choose color based on battery level
  GColor bar_color;
  if (settings.ShowCharging && is_charging) {
    bar_color = settings.BatteryChargingColor;
  } else if (battery_level <= settings.BatteryLowPercent) {
    bar_color = settings.BatteryLowColor;
  } else if (battery_level <= settings.BatteryMidPercent) {
    bar_color = settings.BatteryMidColor;
  } else {
    bar_color = settings.BatteryFullColor;
  }

  // Draw the filled bar inside the border
  graphics_context_set_fill_color(ctx, bar_color);
  graphics_fill_rect(ctx, GRect(2, 2, bar_width, bounds.size.h - 4), 1, GCornerNone);
}

static void watch_battery_update_proc(Layer *layer, GContext *ctx) {
  battery_update_proc(layer, ctx, s_battery_level, s_battery_charging);
}

static void phone_battery_update_proc(Layer *layer, GContext *ctx) {
  battery_update_proc(layer, ctx, settings.PhoneBattery, settings.PhoneCharging);
}

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(text_layer_get_layer(s_moon_layer), !connected || !settings.ShowMoon);
  layer_set_hidden(text_layer_get_layer(s_bt_icon_layer), connected);
  if (!connected) {
    //phone was disconnected
    if (settings.BluetoothVibrate) {
      vibes_long_pulse();
    }
    #if defined(PBL_SPEAKER)
      if (settings.BluetoothSound) {
        static const SpeakerNote s_double_beep_sawtooth[] = {
          { .midi_note = 96, .waveform = SpeakerWaveformSawtooth,  .duration_ms = 150 },
          { .midi_note = 0,   .waveform = SpeakerWaveformSawtooth,  .duration_ms = 100 },
          { .midi_note = 96, .waveform = SpeakerWaveformSawtooth,  .duration_ms = 150 },
          { .midi_note = 0,   .waveform = SpeakerWaveformSawtooth,  .duration_ms = 100 }
        };
        speaker_play_notes(s_double_beep_sawtooth, ARRAY_LENGTH(s_double_beep_sawtooth), settings.Volume);
      }
    #endif
  }
}

// AppMessage received handler
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Save previous values to detect actual changes
  bool prev_ShowSteps = settings.ShowSteps;
  bool prev_ShowHR = settings.ShowHR;
  bool prev_ShowSun = settings.ShowSun;
  bool prev_ShowMoon = settings.ShowMoon;
  bool prev_NightTheme = settings.NightTheme;
  bool prev_ShowWeather = settings.ShowWeather;
  bool prev_TemperatureUnit = settings.TemperatureUnit;
  bool prev_ShowPhoneBattery = settings.ShowPhoneBattery;
  bool prev_AltDate = settings.AltDate;
  int prev_Lat = settings.Latitude;
  int prev_Lon = settings.Longitude;

  Tuple *js_ready_t = dict_find(iterator, MESSAGE_KEY_JSReady);
  if(js_ready_t) {
    // PebbleKit JS is ready! Safe to send messages
    s_js_ready = true;
  }

  // Check for Clay settings data
  Tuple *bl_color_day_t = dict_find(iterator, MESSAGE_KEY_BacklightColorDay);
  if (bl_color_day_t) {
    settings.BacklightColorDay = bl_color_day_t->value->int32;
  }
  Tuple *bg_color_day_t = dict_find(iterator, MESSAGE_KEY_BackgroundColorDay);
  if (bg_color_day_t) {
    settings.BackgroundColorDay = GColorFromHEX(bg_color_day_t->value->int32);
  }
  Tuple *text_color_day_t = dict_find(iterator, MESSAGE_KEY_TextColorDay);
  if (text_color_day_t) {
    settings.TextColorDay = GColorFromHEX(text_color_day_t->value->int32);
  }
  Tuple *time_color_day_t = dict_find(iterator, MESSAGE_KEY_TimeColorDay);
  if (time_color_day_t) {
    settings.TimeColorDay = GColorFromHEX(time_color_day_t->value->int32);
  }
  Tuple *date_color_day_t = dict_find(iterator, MESSAGE_KEY_DateColorDay);
  if (date_color_day_t) {
    settings.DateColorDay = GColorFromHEX(date_color_day_t->value->int32);
  }
  Tuple *weather_color_day_t = dict_find(iterator, MESSAGE_KEY_WeatherColorDay);
  if (weather_color_day_t) {
    settings.WeatherColorDay = GColorFromHEX(weather_color_day_t->value->int32);
  }
  Tuple *health_color_day_t = dict_find(iterator, MESSAGE_KEY_HealthColorDay);
  if (health_color_day_t) {
    settings.HealthColorDay = GColorFromHEX(health_color_day_t->value->int32);
  }
  Tuple *sun_color_day_t = dict_find(iterator, MESSAGE_KEY_SunColorDay);
  if (sun_color_day_t) {
    settings.SunColorDay = GColorFromHEX(sun_color_day_t->value->int32);
  }
  Tuple *moon_color_day_t = dict_find(iterator, MESSAGE_KEY_MoonColorDay);
  if (moon_color_day_t) {
    settings.MoonColorDay = GColorFromHEX(moon_color_day_t->value->int32);
  }
  Tuple *battery_outline_color_day_t = dict_find(iterator, MESSAGE_KEY_BatteryOutlineColorDay);
  if (battery_outline_color_day_t) {
    settings.BatteryOutlineColorDay = GColorFromHEX(battery_outline_color_day_t->value->int32);
  }
  Tuple *battery_charging_color_day_t = dict_find(iterator, MESSAGE_KEY_BatteryChargingColorDay);
  if (battery_charging_color_day_t) {
    settings.BatteryChargingColorDay = GColorFromHEX(battery_charging_color_day_t->value->int32);
  }
  Tuple *battery_full_color_day_t = dict_find(iterator, MESSAGE_KEY_BatteryFullColorDay);
  if (battery_full_color_day_t) {
    settings.BatteryFullColorDay = GColorFromHEX(battery_full_color_day_t->value->int32);
  }
  Tuple *battery_mid_color_day_t = dict_find(iterator, MESSAGE_KEY_BatteryMidColorDay);
  if (battery_mid_color_day_t) {
    settings.BatteryMidColorDay = GColorFromHEX(battery_mid_color_day_t->value->int32);
  }
  Tuple *battery_low_color_day_t = dict_find(iterator, MESSAGE_KEY_BatteryLowColorDay);
  if (battery_low_color_day_t) {
    settings.BatteryLowColorDay = GColorFromHEX(battery_low_color_day_t->value->int32);
  }
  Tuple *bl_color_night_t = dict_find(iterator, MESSAGE_KEY_BacklightColorNight);
  if (bl_color_night_t) {
    settings.BacklightColorNight = bl_color_night_t->value->int32;
  }
  Tuple *bg_color_night_t = dict_find(iterator, MESSAGE_KEY_BackgroundColorNight);
  if (bg_color_night_t) {
    settings.BackgroundColorNight = GColorFromHEX(bg_color_night_t->value->int32);
  }
  Tuple *text_color_night_t = dict_find(iterator, MESSAGE_KEY_TextColorNight);
  if (text_color_night_t) {
    settings.TextColorNight = GColorFromHEX(text_color_night_t->value->int32);
  }
  Tuple *time_color_night_t = dict_find(iterator, MESSAGE_KEY_TimeColorNight);
  if (time_color_night_t) {
    settings.TimeColorNight = GColorFromHEX(time_color_night_t->value->int32);
  }
  Tuple *date_color_night_t = dict_find(iterator, MESSAGE_KEY_DateColorNight);
  if (date_color_night_t) {
    settings.DateColorNight = GColorFromHEX(date_color_night_t->value->int32);
  }
  Tuple *weather_color_night_t = dict_find(iterator, MESSAGE_KEY_WeatherColorNight);
  if (weather_color_night_t) {
    settings.WeatherColorNight = GColorFromHEX(weather_color_night_t->value->int32);
  }
  Tuple *health_color_night_t = dict_find(iterator, MESSAGE_KEY_HealthColorNight);
  if (health_color_night_t) {
    settings.HealthColorNight = GColorFromHEX(health_color_night_t->value->int32);
  }
  Tuple *sun_color_night_t = dict_find(iterator, MESSAGE_KEY_SunColorNight);
  if (sun_color_night_t) {
    settings.SunColorNight = GColorFromHEX(sun_color_night_t->value->int32);
  }
  Tuple *moon_color_night_t = dict_find(iterator, MESSAGE_KEY_MoonColorNight);
  if (moon_color_night_t) {
    settings.MoonColorNight = GColorFromHEX(moon_color_night_t->value->int32);
  }
  Tuple *battery_outline_color_night_t = dict_find(iterator, MESSAGE_KEY_BatteryOutlineColorNight);
  if (battery_outline_color_night_t) {
    settings.BatteryOutlineColorNight = GColorFromHEX(battery_outline_color_night_t->value->int32);
  }
  Tuple *battery_charging_color_night_t = dict_find(iterator, MESSAGE_KEY_BatteryChargingColorNight);
  if (battery_charging_color_night_t) {
    settings.BatteryChargingColorNight = GColorFromHEX(battery_charging_color_night_t->value->int32);
  }
  Tuple *battery_full_color_night_t = dict_find(iterator, MESSAGE_KEY_BatteryFullColorNight);
  if (battery_full_color_night_t) {
    settings.BatteryFullColorNight = GColorFromHEX(battery_full_color_night_t->value->int32);
  }
  Tuple *battery_mid_color_night_t = dict_find(iterator, MESSAGE_KEY_BatteryMidColorNight);
  if (battery_mid_color_night_t) {
    settings.BatteryMidColorNight = GColorFromHEX(battery_mid_color_night_t->value->int32);
  }
  Tuple *battery_low_color_night_t = dict_find(iterator, MESSAGE_KEY_BatteryLowColorNight);
  if (battery_low_color_night_t) {
    settings.BatteryLowColorNight = GColorFromHEX(battery_low_color_night_t->value->int32);
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
  if (prev_AltDate != settings.AltDate) {
    update_date();
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
  }
  Tuple *show_hr_t = dict_find(iterator, MESSAGE_KEY_ShowHR);
  if (show_hr_t) {
    settings.ShowHR = show_hr_t->value->int32 == 1;
  }
  if ((prev_ShowSteps != settings.ShowSteps) || (prev_ShowHR != settings.ShowHR)) {
    update_health();
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
  Tuple *show_charging_t = dict_find(iterator, MESSAGE_KEY_ShowCharging);
  if (show_charging_t) {
    settings.ShowCharging = show_charging_t->value->int32 == 1;
  }
  Tuple *battery_mid_percent_t = dict_find(iterator, MESSAGE_KEY_BatteryMidPercent);
  if (battery_mid_percent_t) {
    settings.BatteryMidPercent = (int)battery_mid_percent_t->value->int32;
  }
  Tuple *battery_low_percent_t = dict_find(iterator, MESSAGE_KEY_BatteryLowPercent);
  if (battery_low_percent_t) {
    settings.BatteryLowPercent = (int)battery_low_percent_t->value->int32;
  }
  Tuple *periodic_vibrate_t = dict_find(iterator, MESSAGE_KEY_PeriodicVibrate);
  if (periodic_vibrate_t) {
    settings.PeriodicVibrate = periodic_vibrate_t->value->int32 == 1;
  }
  Tuple *periodic_sound_t = dict_find(iterator, MESSAGE_KEY_PeriodicSound);
  if (periodic_sound_t) {
    settings.PeriodicSound = periodic_sound_t->value->int32 == 1;
  }
  Tuple *bluetooth_vibrate_t = dict_find(iterator, MESSAGE_KEY_BluetoothVibrate);
  if (bluetooth_vibrate_t) {
    settings.BluetoothVibrate = bluetooth_vibrate_t->value->int32 == 1;
  }
  Tuple *bluetooth_sound_t = dict_find(iterator, MESSAGE_KEY_BluetoothSound);
  if (bluetooth_sound_t) {
    settings.BluetoothSound = bluetooth_sound_t->value->int32 == 1;
  }
  Tuple *volume_t = dict_find(iterator, MESSAGE_KEY_Volume);
  if (volume_t) {
    settings.Volume = (int)volume_t->value->int32;
  }

  // check for manual coordinates
  Tuple *man_lat_t = dict_find(iterator, MESSAGE_KEY_Latitude);
  if (man_lat_t) {
    settings.Latitude = parse_coordinates(man_lat_t->value->cstring);
  }
  Tuple *man_lon_t = dict_find(iterator, MESSAGE_KEY_Longitude);
  if (man_lon_t) {
    settings.Longitude = parse_coordinates(man_lon_t->value->cstring);
  }
  if (settings.Latitude && settings.Longitude && (strcmp(man_lat_t->value->cstring,"") != 0) && (strcmp(man_lon_t->value->cstring,"") != 0)) {
    settings.ManualCoordinates = true;
  } else {
    settings.ManualCoordinates = false;
  }

  // Check for weather data
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  if (temp_tuple) {
    if (settings.TemperatureUnit) {
      settings.WeatherTemp = ((int)temp_tuple->value->int32 * 9 / 5) + 32;
    } else {
      settings.WeatherTemp = (int)temp_tuple->value->int32;
    }
  }
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);
  if (conditions_tuple) {
    settings.WeatherIcon = (int)conditions_tuple->value->int32;
  }
  if (temp_tuple || conditions_tuple) {
    update_weather();
  }

  // Check for sun info
  Tuple *sunrise_tuple = dict_find(iterator, MESSAGE_KEY_SUNRISE);
  Tuple *sunset_tuple = dict_find(iterator, MESSAGE_KEY_SUNSET);
  if (sunrise_tuple && sunset_tuple) {
    settings.SunriseTime = (int)sunrise_tuple->value->int32;
    settings.SunsetTime = (int)sunset_tuple->value->int32;
    update_sun();
  }
  Tuple *moon_tuple = dict_find(iterator, MESSAGE_KEY_MOONPHASE);
  if (moon_tuple) {
    settings.MoonPhase = (int)moon_tuple->value->int32;
    update_moon();
  }

  // check for battery data
  Tuple *battery_tuple = dict_find(iterator, MESSAGE_KEY_BATTERY);
  if (battery_tuple) {
    settings.PhoneBattery = (int)battery_tuple->value->int32;
  }
  Tuple *charging_tuple = dict_find(iterator, MESSAGE_KEY_CHARGING);
  if (charging_tuple) {
    settings.PhoneCharging = charging_tuple->value->int32 == 1;
  }
  if (battery_tuple || charging_tuple) {
    layer_mark_dirty(s_phone_battery_layer);
  }

  // Save and apply if any settings were changed
  if (
    bg_color_day_t || bg_color_night_t || night_theme_t || 
    text_color_day_t || text_color_night_t || bl_color_day_t || bl_color_night_t ||
    time_color_day_t || date_color_day_t || weather_color_day_t || health_color_day_t || 
    sun_color_day_t || moon_color_day_t || battery_outline_color_day_t || battery_charging_color_day_t || 
    battery_full_color_day_t || battery_mid_color_day_t || battery_low_color_day_t || 
    time_color_night_t || date_color_night_t || weather_color_night_t || health_color_night_t || 
    sun_color_night_t || moon_color_night_t || battery_outline_color_night_t || battery_charging_color_night_t || 
    battery_full_color_night_t || battery_mid_color_night_t || battery_low_color_night_t || 
    show_date_t || show_date2_t || alt_date_t || show_steps_t || show_hr_t || 
    show_weather_t || temp_unit_t || weahter_interval_t || show_sun_t || show_moon_t || man_lat_t || man_lon_t || 
    show_charging_t || battery_mid_percent_t || battery_low_percent_t || 
    show_phone_battery_t || periodic_vibrate_t || periodic_sound_t || 
    bluetooth_vibrate_t || bluetooth_sound_t || volume_t || js_ready_t) {
    
    // if show battery was toggled
    if (prev_ShowPhoneBattery != settings.ShowPhoneBattery) {
      int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
      int bar_height = (PBL_DISPLAY_HEIGHT / 24);
      int bar_width = PBL_DISPLAY_WIDTH / 1.1;
      int bar_x = (PBL_DISPLAY_WIDTH - bar_width) / 2;
      int bar_y = PBL_IF_ROUND_ELSE(PBL_DISPLAY_HEIGHT - (bar_offset + (PBL_DISPLAY_HEIGHT / 3.75)), PBL_DISPLAY_HEIGHT - (bar_offset - (PBL_DISPLAY_HEIGHT / 12)));
      if (settings.ShowPhoneBattery) {
        bar_width = (bar_width / 2) - (bar_x / 2);
      }
      layer_set_frame(s_battery_layer, GRect(bar_x, bar_y, bar_width, bar_height));
      layer_mark_dirty(s_battery_layer);
    }

    //resize weather and health layers if steps and/or HR is toggled
    if (
    ((prev_ShowSteps != settings.ShowSteps) || (prev_ShowHR != settings.ShowHR)) &&
    ((prev_ShowSteps && prev_ShowHR) != (settings.ShowSteps && settings.ShowHR)) &&
    (PBL_DISPLAY_HEIGHT >= 228) ) {
      int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
      int bar_y = PBL_IF_ROUND_ELSE(PBL_DISPLAY_HEIGHT - (bar_offset + (PBL_DISPLAY_HEIGHT / 3.75)), PBL_DISPLAY_HEIGHT - (bar_offset - (PBL_DISPLAY_HEIGHT / 12)));
      int info_height = 28;
      int info_padding = 10;
      // weather
      int weather_x = 0;
      int weather_y = PBL_IF_ROUND_ELSE(bar_y + (15), bar_y - (info_height * 2) - (PBL_DISPLAY_HEIGHT / 15));
      int weather_width = ((PBL_DISPLAY_WIDTH / 10) * 4);
      if (settings.ShowSteps && settings.ShowHR) {
        weather_width = (weather_width * 0.75);
      }
      // weather icon
      int weather_icon_x = weather_width;
      int weather_icon_y = weather_y + (info_padding * 0.75);
      int weather_icon_width = ((PBL_DISPLAY_WIDTH / 10) * 2);
      // health
      int health_x = weather_icon_x + weather_icon_width;
      int health_y = weather_y;
      int health_width = ((PBL_DISPLAY_WIDTH / 10) * 4);
      if (settings.ShowSteps && settings.ShowHR) {
        health_width = (health_width * 1.25);
      }
      layer_set_frame(text_layer_get_layer(s_weather_layer), GRect(weather_x, weather_y, weather_width, (info_height + 4)));
      layer_mark_dirty(text_layer_get_layer(s_weather_layer));
      layer_set_frame(text_layer_get_layer(s_weather_icon_layer), GRect(weather_icon_x, weather_icon_y, weather_icon_width, (info_height + 4)));
      layer_mark_dirty(text_layer_get_layer(s_weather_icon_layer));
      layer_set_frame(text_layer_get_layer(s_health_layer), GRect(health_x, health_y, health_width, (info_height + 4)));
      layer_mark_dirty(text_layer_get_layer(s_health_layer));
    }
    
    prv_save_settings();
    prv_update_display();

    // Request data when a setting was changed
    settings.updateCoordinates = (prev_Lat != settings.Latitude) ||
                                 (prev_Lon != settings.Longitude);
    settings.requestSun = settings.requestSun || 
                          (!prev_ShowSun && settings.ShowSun) ||
                          (!prev_ShowMoon && settings.ShowMoon) ||
                          (!prev_NightTheme && settings.NightTheme) ||
                          (settings.updateCoordinates && (settings.ShowSun || settings.ShowMoon || settings.NightTheme));
    settings.requestWeather = settings.requestWeather || 
                              (((prev_TemperatureUnit != settings.TemperatureUnit) || !prev_ShowWeather || settings.updateCoordinates) && settings.ShowWeather);
    settings.requestBattery = (!prev_ShowPhoneBattery && settings.ShowPhoneBattery);
    settings.unsibscribeBattery = (prev_ShowPhoneBattery && !settings.ShowPhoneBattery);
    if (settings.requestSun || settings.requestWeather || settings.requestBattery || settings.unsibscribeBattery || settings.updateCoordinates) {
      request_js();
    }
  } else if (temp_tuple || conditions_tuple || sunrise_tuple || sunset_tuple || moon_tuple || battery_tuple || charging_tuple) {
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
  layer_set_hidden(text_layer_get_layer(s_health_layer), true);
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
  int bar_y = PBL_IF_ROUND_ELSE(bounds.size.h - (bar_offset + (bounds.size.h / 3.75)), bounds.size.h - (bar_offset - (bounds.size.h / 12)));

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
    layer_set_hidden(text_layer_get_layer(s_health_layer), true);
    layer_set_hidden(text_layer_get_layer(s_weather_layer), true);
    layer_set_hidden(text_layer_get_layer(s_weather_icon_layer), true);
    layer_set_hidden(text_layer_get_layer(s_sunrise_layer), true);
    layer_set_hidden(text_layer_get_layer(s_sunset_layer), true);
    layer_set_hidden(text_layer_get_layer(s_moon_layer), true);
  } else {
    layer_set_hidden(text_layer_get_layer(s_bt_icon_layer),
      connection_service_peek_pebble_app_connection());
    layer_set_hidden(text_layer_get_layer(s_health_layer), (!settings.ShowSteps && !settings.ShowHR));
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
  int weather_padding;
  s_info_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  info_padding = 10;
  info_height = 28;
  s_date_font = s_info_font;
  date_padding = info_padding;
  date_height = info_height;
  #if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_GABBRO)
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TALLBOLD_70));
    time_padding = 2;
    time_height = 72;
    s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_22));
    s_bt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_20));
    weather_padding = 6;
  #else
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TALLBOLD_49));
    time_padding = 2;
    time_height = 49;
    s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_18));
    s_bt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
    weather_padding = 8;
  #endif

  // Position the time + date block
  int date2_y = 0;
  int date_y = date2_y;
  if (PBL_DISPLAY_HEIGHT >= 228) {
    date_y = date_y + date_height;
  }
  int time_y = date_y + date_height + date_padding - time_padding;

  // Create the time TextLayer
  s_time_layer = text_layer_create(
      GRect(0, time_y, bounds.size.w, time_height));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, settings.TimeColor);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create the date TextLayer
  s_date_layer = text_layer_create(
      GRect(0, date_y, bounds.size.w, (info_height + 4)));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.DateColor);
  text_layer_set_font(s_date_layer, s_info_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  s_date2_layer = text_layer_create(
      GRect(0, date2_y, bounds.size.w, (info_height + 4)));
  text_layer_set_background_color(s_date2_layer, GColorClear);
  text_layer_set_text_color(s_date2_layer, settings.DateColor);
  text_layer_set_font(s_date2_layer, s_info_font);
  text_layer_set_text_alignment(s_date2_layer, GTextAlignmentCenter);

  // Create battery meter Layer
  int bar_offset = (PBL_DISPLAY_HEIGHT / 6);
  int bar_height = (PBL_DISPLAY_HEIGHT / 24);
  int bar_width = bounds.size.w / 1.1;
  int bar_x = (bounds.size.w - bar_width) / 2;
  int bar_y = PBL_IF_ROUND_ELSE(bounds.size.h - (bar_offset + (bounds.size.h / 3.75)), bounds.size.h - (bar_offset - (bounds.size.h / 12)));
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
  int weather_y = PBL_IF_ROUND_ELSE(bar_y + (bar_height * 1.4), bar_y - (info_height * 2) - (bounds.size.h / 15));
  int weather_width = ((bounds.size.w / 10) * 4);
  int weather_x = 0;
  if ((settings.ShowSteps && settings.ShowHR) && (PBL_DISPLAY_HEIGHT >= 228)) {
    weather_width = (weather_width * 0.75);
  }
  s_weather_layer = text_layer_create(
      GRect(weather_x, weather_y, weather_width, (info_height + 4)));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, settings.WeatherColor);
  text_layer_set_font(s_weather_layer, s_info_font);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentRight);

  // Create weather icon TextLayer
  int weather_icon_y = weather_y + weather_padding;
  int weather_icon_width = ((bounds.size.w / 10) * 2);
  int weather_icon_x = weather_width;
  s_weather_icon_layer = text_layer_create(
      GRect(weather_icon_x, weather_icon_y, weather_icon_width, (info_height + 4)));
  text_layer_set_background_color(s_weather_icon_layer, GColorClear);
  text_layer_set_text_color(s_weather_icon_layer, settings.WeatherColor);
  text_layer_set_font(s_weather_icon_layer, s_weather_font);
  text_layer_set_text_alignment(s_weather_icon_layer, GTextAlignmentCenter);

  // Create health TextLayer
  int health_y = weather_y;
  int health_width = ((bounds.size.w / 10) * 4);
  int health_x = weather_icon_x + weather_icon_width;
  if ((settings.ShowSteps && settings.ShowHR) && (PBL_DISPLAY_HEIGHT >= 228)) {
    health_width = (health_width * 1.25);
  }
  s_health_layer = text_layer_create(
      GRect(health_x, health_y, health_width, (info_height + 4)));
  text_layer_set_background_color(s_health_layer, GColorClear);
  text_layer_set_text_color(s_health_layer, settings.HealthColor);
  text_layer_set_font(s_health_layer, s_info_font);
  text_layer_set_text_alignment(s_health_layer, GTextAlignmentLeft);

  // Create sun TextLayer
  int sun_y = weather_y + info_height;
  s_sunrise_layer = text_layer_create(
      GRect(0, sun_y, ((bounds.size.w / 5) * 2), (info_height + 4)));
  text_layer_set_background_color(s_sunrise_layer, GColorClear);
  text_layer_set_text_color(s_sunrise_layer, settings.SunColor);
  text_layer_set_font(s_sunrise_layer, s_info_font);
  text_layer_set_text_alignment(s_sunrise_layer, GTextAlignmentRight);
  s_sunset_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 3), sun_y, ((bounds.size.w / 5) * 2), (info_height + 4)));
  text_layer_set_background_color(s_sunset_layer, GColorClear);
  text_layer_set_text_color(s_sunset_layer, settings.SunColor);
  text_layer_set_font(s_sunset_layer, s_info_font);
  text_layer_set_text_alignment(s_sunset_layer, GTextAlignmentLeft);

  // Create the moon layer
  int moon_y = sun_y + weather_padding;
  //moon_y = 0;
  s_moon_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 2), moon_y, ((bounds.size.w / 5) * 1), (info_height + 4)));
  text_layer_set_background_color(s_moon_layer, GColorClear);
  text_layer_set_text_color(s_moon_layer, settings.MoonColor);
  text_layer_set_font(s_moon_layer, s_weather_font);
  text_layer_set_text_alignment(s_moon_layer, GTextAlignmentCenter);

  // Create the Bluetooth icon
  int bt_y = sun_y + (info_padding * 1.1);
  s_bt_icon_layer = text_layer_create(
      GRect(((bounds.size.w / 5) * 2), bt_y, ((bounds.size.w / 5) * 1), (info_height + 4)));
  text_layer_set_background_color(s_bt_icon_layer, GColorClear);
  text_layer_set_text_color(s_bt_icon_layer, settings.TimeColor);
  text_layer_set_font(s_bt_icon_layer, s_bt_font);
  text_layer_set_text_alignment(s_bt_icon_layer, GTextAlignmentCenter);
  text_layer_set_text(s_bt_icon_layer, "z");

  // Add layers to the Window
  layer_add_child(s_window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_date2_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_weather_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_weather_icon_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_health_layer));
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
  text_layer_destroy(s_health_layer);
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
  if (settings.ShowSteps || settings.ShowHR){
    update_health();
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
  const int inbox_size = 512;
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
