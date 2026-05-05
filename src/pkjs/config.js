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
        "messageKey": "ShowDate2",
        "label": "Show additional Day/Year",
        "description": "Only for larger screens e.g. emery",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "AltDate",
        "label": "Alternate Date Format",
        "description": "Toggle between `Day Mon DD` and `YYYY-MM-DD`",
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
        "description": "Leave both blank to use GPS location for weather & sun times. You can use <a href =https://www.google.com/maps>Google Maps</a> or <a href =https://www.openstreetmap.org/>OpenStreetMap</a> to find latitude & longitude.",
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
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
