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
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
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
        "messageKey": "TextColorDay",
        "label": "Text Color",
        "defaultValue": "0x000000",
        "allowGray": true
      },
      {
        "type": "toggle",
        "messageKey": "NightTheme",
        "label": "Alternate Nighttime Colors",
        "defaultValue": false
      },
      {
        "type": "color",
        "messageKey": "BackgroundColorNight",
        "label": "Nighttime Background Color",
        "defaultValue": "0x000000",
        "allowGray": true
      },
      {
        "type": "color",
        "messageKey": "TextColorNight",
        "label": "Nighttime Text Color",
        "defaultValue": "0xFFFFFF",
        "allowGray": true
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
        "messageKey": "ShowDate",
        "label": "Show Date",
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
        "type": "toggle",
        "messageKey": "ShowSteps",
        "label": "Show Steps",
        "defaultValue": false
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
