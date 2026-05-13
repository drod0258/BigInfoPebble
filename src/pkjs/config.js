module.exports = [
  {
    "type": "heading",
    "defaultValue": "Watchface Settings"
  },
  {
    "type": "text",
    "defaultValue": "Customize your watchface appearance and preferences."
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Theme"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColorDay",
        "label": "Background Color",
        "defaultValue": "0xFFFFFF",
        "allowGray": true
      },
      {
        "type": "color",
        "messageKey": "TimeColorDay",
        "label": "Time Color",
        "defaultValue": "0x000000"
      },
      {
        "type": "color",
        "messageKey": "DateColorDay",
        "label": "Date Color",
        "defaultValue": "0x000000",
      },
      {
        "type": "color",
        "messageKey": "WeatherColorDay",
        "label": "Weather Color",
        "defaultValue": "0x000000",
      },
      {
        "type": "color",
        "messageKey": "HealthColorDay",
        "label": "Health Color",
        "defaultValue": "0x000000",
      },
      {
        "type": "color",
        "messageKey": "SunColorDay",
        "label": "Sun Time Color",
        "defaultValue": "0x000000",
      },
      {
        "type": "color",
        "messageKey": "MoonColorDay",
        "label": "Moon Color",
        "defaultValue": "0x000000",
      },
      {
        "type": "color",
        "messageKey": "BatteryColorDay",
        "label": "Battery Color",
        "defaultValue": "0x000000",
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Night Theme"
      },
      {
        "type": "toggle",
        "messageKey": "NightTheme",
        "label": "Enable Night Theme After Sunset",
        "defaultValue": false
      },
      {
        "type": "color",
        "messageKey": "BackgroundColorNight",
        "label": "Night Background Color",
        "defaultValue": "0x000000",
        "allowGray": true
      },
      {
        "type": "color",
        "messageKey": "TimeColorNight",
        "label": "Night Time Color",
        "defaultValue": "0xFFFFFF"
      },
      {
        "type": "color",
        "messageKey": "DateColorNight",
        "label": "Night Date Color",
        "defaultValue": "0xFFFFFF",
      },
      {
        "type": "color",
        "messageKey": "WeatherColorNight",
        "label": "Night Weather Color",
        "defaultValue": "0xFFFFFF",
      },
      {
        "type": "color",
        "messageKey": "HealthColorNight",
        "label": "Night Health Color",
        "defaultValue": "0xFFFFFF",
      },
      {
        "type": "color",
        "messageKey": "SunColorNight",
        "label": "Night Sun Time Color",
        "defaultValue": "0xFFFFFF",
      },
      {
        "type": "color",
        "messageKey": "MoonColorNight",
        "label": "Night Moon Color",
        "defaultValue": "0xFFFFFF",
      },
      {
        "type": "color",
        "messageKey": "BatteryColorNight",
        "label": "Night Battery Color",
        "defaultValue": "0xFFFFFF",
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Date"
      },
      {
        "type": "toggle",
        "messageKey": "ShowDate",
        "label": "Show Date",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "ShowDate2",
        "label": "Show additional Date",
        "description": "Day/Year for larger screens (Time 2)",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "AltDate",
        "label": "Alternate Date Format",
        "description": "Toggle between `Day Mon DD` and `YYYY-MM-DD`",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Health"
      },
      {
        "type": "toggle",
        "messageKey": "ShowSteps",
        "label": "Show Steps",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "ShowHR",
        "label": "Show Heartrate",
        "defaultValue": false,
        "description": "Health data is updated every minute"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Location"
      },
      {
        "type": "toggle",
        "messageKey": "ShowSun",
        "label": "Show Sunrise/Sunset",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "ShowMoon",
        "label": "Show Moonphase",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "ShowWeather",
        "label": "Show Weather",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "TemperatureUnit",
        "label": "Use Fahrenheit",
        "defaultValue": false
      },
      {
        "type": "slider",
        "messageKey": "WeatherInterval",
        "label": "Weather Update Interval (hours)",
        "defaultValue": 3,
        "min": 1,
        "max": 6,
        "step": 1
      },
      {
        "type": "input",
        "messageKey": "Latitude",
        "defaultValue": "",
        "label": "Manual Location - Latitude",
        "attributes": {
          "placeholder": "eg: 40.7127 (leave blank to use GPS)",
          "type": "number",
          "min": "-90",
          "max": "90",
          "step": ".000001"
        }
      },
      {
        "type": "input",
        "messageKey": "Longitude",
        "defaultValue": "",
        "label": "Manual Location - Longitude",
        "description": "Leave both blank to use GPS location for weather & sun times",
        "attributes": {
          "placeholder": "eg: -74.0061 (leave blank to use GPS)",
          "type": "number",
          "min": "-180",
          "max": "180",
          "step": ".000001"
        }
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Preferences"
      },
      {
        "type": "toggle",
        "messageKey": "ShowPhoneBattery",
        "label": "Show Phone Battery",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "PeriodicVibrate",
        "label": "Vibrate Hourly",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "BluetoothVibrate",
        "label": "Vibrate when Bluetooth Disconnects",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
