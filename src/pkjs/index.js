// Import the Clay package
var Clay = require('@rebble/clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);

// Helper function for XMLHttpRequest
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

// Sun & Moon functions
// shortcuts for easier to read formulas
var PI   = Math.PI,
    sin  = Math.sin,
    cos  = Math.cos,
    tan  = Math.tan,
    asin = Math.asin,
    atan = Math.atan2,
    acos = Math.acos,
    rad  = PI / 180;
function rev(x) {
  return x - Math.floor(x / 360.0) * 360.0;
}
// date/time constants and conversions
var dayMs = 1000 * 60 * 60 * 24,
    J1970 = 2440588,
    J2000 = 2451545;
function toDays(date) {
  return (date.getTime() / dayMs) + J1970 - J2000;
}
// general calculations for position
function getObliquity(d) {
  return 23.4393 - 3.563e-7 * d;
}
function getRightAscension(l, b, o) {
  return atan(sin(l) * cos(o) - tan(b) * sin(o), cos(l));
}
function getDeclination(l, b, o) {
  return asin(sin(b) * cos(o) + cos(b) * sin(o) * sin(l));
}
// sun calculation
var sunCalc = {
  getTimes: function(date, lat, lon) {
    var d = toDays(date);
    // Mean anomaly, mean longitude, and obliquity
    var M = rev(357.528 + 0.9856003 * d);
    var L = rev(280.460 + 0.9856474 * d);
    var obl = getObliquity(d);
    // Ecliptic longitude
    var lambda = rev(L + 1.915 * sin(M * rad) + 0.020 * sin(2 * M * rad));
    // Right ascension and Declination
    var ra = rev(atan(cos(obl * rad) * sin(lambda * rad), cos(lambda * rad)) / rad);
    var dec = (asin(sin(obl * rad) * sin(lambda * rad)) / rad);
    // Sidereal time at Greenwich
    var GMST0 = rev(L + 180);
    var UT_noon = (ra - GMST0 - lon) / 15.0;
    while (UT_noon < 0) UT_noon += 24.0;
    while (UT_noon >= 24) UT_noon -= 24.0;
    // Hour angle for sunrise/sunset (altit = -0.833 for refraction + radius)
    var altit = -0.8333;
    var cos_h = (sin(altit * rad) - sin(lat * rad) * sin(dec * rad)) / (cos(lat * rad) * cos(dec * rad));
    // Handle edge cases for polar regions
    if (cos_h > 1.0) return { rise: null, set: null };  // Always below horizon
    if (cos_h < -1.0) return { rise: null, set: null }; // Always above horizon
    var h = (acos(cos_h) / rad) / 15.0;
    return {
      rise: this.dateFromUTCHours(date, UT_noon - h),
      set: this.dateFromUTCHours(date, UT_noon + h)
    };
  },

  dateFromUTCHours: function(baseDate, utHours) {
    var d = new Date(baseDate);
    d.setUTCHours(Math.floor(utHours));
    d.setUTCMinutes(Math.floor((utHours * 60) % 60));
    d.setUTCSeconds(Math.floor((utHours * 3600) % 60));
    return d;
  }
};
// moon calculations
function getSunCoords(d) {
  var M = rad * (357.5291 + 0.98560028 * d),
      C = rad * (1.9148 * sin(M) + 0.02 * sin(2 * M) + 0.0003 * sin(3 * M)),
      L = M + C + (rad * 102.9372) + PI;
      obl = rad * 23.4393; // obliquity of the Earth
  return {
    dec: getDeclination(L, 0, obl),
    ra: getRightAscension(L, 0, obl)
  };
}
function getMoonCoords(d) { // geocentric ecliptic coordinates of the moon
  var L = rad * (218.316 + 13.176396 * d), // ecliptic longitude
      M = rad * (134.963 + 13.064993 * d), // mean anomaly
      F = rad * (93.272 + 13.229350 * d),  // mean distance
      l  = L + rad * 6.289 * sin(M), // longitude
      b  = rad * 5.128 * sin(F),     // latitude
      dt = 385001 - 20905 * cos(M);  // distance to the moon in km
      obl = rad * 23.4393; // obliquity of the Earth
  return {
    ra: getRightAscension(l, b, obl),
    dec: getDeclination(l, b, obl),
    dist: dt
  };
}
function getMoonIllumination(date) {
  var d = toDays(date),
      s = getSunCoords(d),
      m = getMoonCoords(d),
      sdist = 149598000,
      phi = acos(sin(s.dec) * sin(m.dec) + cos(s.dec) * cos(m.dec) * cos(s.ra - m.ra)),
      inc = atan(sdist * sin(phi), m.dist - sdist * cos(phi)),
      angle = atan(cos(s.dec) * sin(s.ra - m.ra), sin(s.dec) * cos(m.dec) -
              cos(s.dec) * sin(m.dec) * cos(s.ra - m.ra));
  return {
    fraction: (1 + cos(inc)) / 2,
    phase: 0.5 + 0.5 * inc * (angle < 0 ? -1 : 1) / Math.PI,
    angle: angle
  };
};

function sunInfo (pos){
  var lat = pos.coords.latitude;
  var lon = pos.coords.longitude;
  console.log("lat: " + lat);
  console.log("lon: " + lon);
  var d = new Date();
  console.log("date: " + d);
  var sun = sunCalc.getTimes(d, lat, lon);
  var sunriseint = sun.rise.getHours() * 100 + sun.rise.getMinutes();
  var sunsetint = sun.set.getHours() * 100 + sun.set.getMinutes();
  console.log("sunriseint: " + sunriseint);
  console.log("sunsetint: " + sunsetint);
  var moon = getMoonIllumination(d);
  var moonphase = Math.round(moon.phase * 28);
  console.log("moonphase: " + moonphase);
  var dictionary = {
    "SUNRISE": sunriseint,
    "SUNSET": sunsetint,
    "MOONPHASE": moonphase,
  };
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log('Suncalc stuff sent to Pebble successfully!');
    },
    function(e) {
      console.log('Error sending suncalc stuff to Pebble!');
    }
  );
}
// End Sun & Moon functions

// battery functions
function sendBatteryLevel(battery, reportInterval) {
  var batteryLevel = Math.floor(battery.level * 100);
  var batteryCharging = 0;
  if (battery.charging) {
    batteryCharging = 1;
  }
  console.log('battery level: ' + batteryLevel);
  console.log('battery charging: ' + batteryCharging);
  var dictionary = {
    "BATTERY": batteryLevel,
    "CHARGING": batteryCharging
  };
  if (batteryLevel % reportInterval == 0) {
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log('Battery sent to Pebble successfully!');
      },
      function(e) {
        console.log('Error sending battery to Pebble!');
      }
    );
  }
}
function batteryLevelSubscribe(battery) {
  // Listen for changes in battery level
  battery.addEventListener('levelchange', function() {
    sendBatteryLevel(battery, 5);
  }, false);
  battery.addEventListener('chargingchange', function() {
    sendBatteryLevel(battery, 1);
  }, false);
  // also send battery level immediately
  sendBatteryLevel(battery, 1);
}
function batteryLevelUnsubscribe(battery) {
  // Stop listening for changes in battery level
  battery.removeEventListener('levelchange', function() {
    sendBatteryLevel(battery, 5);
  }, false);
  battery.removeEventListener('chargingchange', function() {
    sendBatteryLevel(battery, 1);
  }, false);
}
function batteryStatusFailure() {
  console.log("Error: Phone Battery function failed to resolve the BatteryManager object.");
}
function getBatteryInfo() {
  // Test for old or new battery API
  if (navigator.battery) {
    console.log('Success: found navigator.battery API');
    batteryLevelSubscribe(navigator.battery);
  } else if (navigator.getBattery) {
    console.log('Success: found navigator.getBattery API');
    navigator.getBattery().then(function(newBattery) {
      batteryLevelSubscribe(newBattery);
    }, batteryStatusFailure);
  } else {
    console.log('Error: no phone battery API found');
  }
}
function stopBattery() {
  // Test for old or new battery API
  if (navigator.battery) {
    console.log('Success: found navigator.battery API');
    batteryLevelUnsubscribe(navigator.battery);
  } else if (navigator.getBattery) {
    console.log('Success: found navigator.getBattery API');
    navigator.getBattery().then(function(newBattery) {
      batteryLevelUnsubscribe(newBattery);
    }, batteryStatusFailure);
  } else {
    console.log('Error: no phone battery API found');
  }
}
// end battery functions

// Convert Open-Meteo weather code to human-readable condition
function weatherCodeToCondition(code) {
  if (code === 0) return 0; //'Clear';
  if (code <= 3) return  code; //'Cloudy';
  if (code <= 48) return 4; //'Fog';
  if (code <= 55) return 5; //'Drizzle';
  if (code <= 57) return 6; //'Fz. Drizzle';
  if (code <= 65) return 7; //'Rain';
  if (code <= 67) return 8; //'Fz. Rain';
  if (code <= 75) return 9; //'Snow';
  if (code <= 77) return 10; //'Snow Grains';
  if (code <= 82) return 11; //'Showers';
  if (code <= 86) return 12; //'Snow Shwrs';
  if (code === 95) return 13; //'T-Storm';
  if (code <= 99) return 14; //'T-Storm';
  return 15; //unknown
}

function weatherInfo(pos) {
  var lat = pos.coords.latitude;
  var lon = pos.coords.longitude;
  console.log("lat: " + lat);
  console.log("lon: " + lon);
  // Construct Open-Meteo API URL
  var url = 'https://api.open-meteo.com/v1/forecast?' +
      'latitude=' + lat +
      '&longitude=' + lon +
      '&current=temperature_2m,weather_code,is_day' +
      '&daily=sunrise,sunset&timezone=auto&forecast_days=1';

  // Send request to Open-Meteo
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      // Temperature (already in Celsius)
      var temperature = Math.round(json.current.temperature_2m);
      console.log('Temperature is ' + temperature);

      // Conditions from weather code
      var conditions = 15 // default to "unknown" weather condition
      conditions = weatherCodeToCondition(json.current.weather_code);
      console.log('Conditions are ' + conditions);

      // change condition based on day or night
      var is_day = json.current.is_day;
      console.log('Is it day: ' + is_day);
      if (is_day === 0) {
        conditions = conditions + 16;
      }

      // sun times                                                                                            
      var sunriseParts = json.daily.sunrise[0].split('T')[1].split(':');                                                                   
      var sunsetParts = json.daily.sunset[0].split('T')[1].split(':');                                                                     
      var sunriseint = parseInt(sunriseParts[0]) * 100 + parseInt(sunriseParts[1]);                                                        
      var sunsetint = parseInt(sunsetParts[0]) * 100 + parseInt(sunsetParts[1]);
      console.log('sunrise: ' + sunriseint);
      console.log('sunset: ' + sunsetint);

      // Assemble dictionary
      var dictionary = {
        "TEMPERATURE": temperature,
        "CONDITIONS": conditions,
        "SUNRISE": sunriseint,
        "SUNSET": sunsetint
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }
  );
}

function weatherError(err) {
  console.log('Error requesting location!');
  var dictionary = {
    "CONDITIONS": 15 // send "unknown" weather code
  };
  // Send to Pebble
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log('Weather error sent to Pebble');
    },
    function(e) {
      console.log('Error sending weather error to Pebble!');
    }
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeatherInfo() {
  if (localStorage.getItem('manualCoordinates') == 1) {
    const mockPosition = {
      coords: {
        latitude: (localStorage.getItem('Latitude') / 1000000),
        longitude: (localStorage.getItem('Longitude') / 1000000),
        altitude: null,
        accuracy: 100,
        altitudeAccuracy: null,
        heading: null,
        speed: null
      },
      timestamp: Date.now()
    };
    console.log('Using manual coordinates for weather info');
    weatherInfo(mockPosition);
  } else {
    console.log('Using GPS for weather info');
    navigator.geolocation.getCurrentPosition(
      weatherInfo,
      weatherError,
      { timeout: 15000, maximumAge: 60000 }
    );
  }
}

function getSunInfo() {
  if (localStorage.getItem('manualCoordinates') == 1) {
    const mockPosition = {
      coords: {
        latitude: (localStorage.getItem('Latitude') / 1000000),
        longitude: (localStorage.getItem('Longitude') / 1000000),
        altitude: null,
        accuracy: 100,
        altitudeAccuracy: null,
        heading: null,
        speed: null
      },
      timestamp: Date.now()
    };
    console.log('Using manual coordinates for sun info');
    sunInfo(mockPosition);
  } else {
    console.log('Using GPS for sun info');
    navigator.geolocation.getCurrentPosition(
      sunInfo,
      locationError,
      { timeout: 15000, maximumAge: 60000 }
    );
  }
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    Pebble.sendAppMessage({'JSReady': 1}); // Update s_js_ready on watch
    // Get the initial data
    //getSunInfo();
    //getWeatherInfo();
    if (localStorage.getItem('phoneBatteryEnabled') == 1) {
      getBatteryInfo();
    }
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    var dict = e.payload;

    // Check for manual coordinates
    if (dict.hasOwnProperty('Latitude') && dict.hasOwnProperty('Longitude')) {
      console.log('manual coordinates recieved');
      var lat = dict['Latitude'];
      var lon = dict['Longitude'];
      // If BOTH fields have content and are not just empty strings save to localStorage
      if (lat !== "" && lon !== "" && lat !== undefined && lon !== undefined) {
        var manualCoordinates = 1;
        localStorage.setItem('Latitude', lat);
        localStorage.setItem('Longitude', lon);
        console.log('latitude: ' + (lat / 1000000));
        console.log('longitude: ' + (lon / 1000000));
      } else {
        var manualCoordinates = 0;
        localStorage.removeItem('Latitude');
        localStorage.removeItem('Longitude');
        console.log('manual coordinates disabled');
      }
      localStorage.setItem('manualCoordinates', manualCoordinates);
    }

    // Check if this is a sun info refresh request
    if (dict['REQUEST_SUN']) {
      getSunInfo();
    }
    // Check if this is a weather refresh request
    if (dict['REQUEST_WEATHER']) {
      getWeatherInfo();
    }
    // Check if this is a battery refresh request
    if (dict['REQUEST_BATTERY']) {
      var batteryToggle = 1;
      localStorage.setItem('phoneBatteryEnabled', batteryToggle);
      getBatteryInfo();
    }
    // Check if this is a battery unsubscribe request
    if (dict['UNSUBSCRIBE_BATTERY']) {
      var batteryToggle = 0;
      localStorage.setItem('phoneBatteryEnabled', batteryToggle);
      stopBattery();
    }
  }
);
