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

function locationError(err) {
  console.log('Error requesting location!');
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

// start plugins
var latestWeather = null;
var latestBattery = null;
var latestSun = null;

function weatherConditionToCode(condition) {
  if (condition === "sun") return 0;
  if (condition === "partly_cloudy") return 1;
  if (condition === "cloudy") return 2;
  if (condition === "light_rain") return 3;
  if (condition === "heavy_rain") return 4;
  if (condition === "light_snow") return 5;
  if (condition === "heavy_snow") return 6;
  if (condition === "rain_and_snow") return 7;
  return 8; //unknown
}

function sendWeather() {
  if (!latestWeather) {
    return;
  }
  Pebble.sendAppMessage(latestWeather,
    function(e) {
      console.log('Weather info sent!');
    },
    function(e) {
      console.log('Error sending weather info!');
    }
  );
}
function sendBattery() {
  if (!latestBattery) {
    return;
  }
  Pebble.sendAppMessage(latestBattery,
    function(e) {
      console.log('Battery info sent!');
    },
    function(e) {
      console.log('Error sending battery info!');
    }
  );
}
function sendSun() {
  if (!latestSun) {
    return;
  }
  Pebble.sendAppMessage(latestSun,
    function(e) {
      console.log('Sun info sent!');
    },
    function(e) {
      console.log('Error sending sun info!');
    }
  );
}

function subscribeToWeather() {
  Pebble.subscribeToSource({
    category: 'weather',
    item: 'location',
    properties: ['temperature', 'condition_code'],
    onData: function(update) {
      var here = update.instances[0];
      if (!here) {
        return;
      }

      // Any property can be missing, so check before using it
      var temperature = here.properties.temperature;
      var condition = here.properties.condition_code;
      if (!temperature || !temperature.numericValue ||
          !condition || !condition.longText) {
        console.log('Weather unavailable');
        return;
      }
      console.log('temperature: ' + JSON.stringify(temperature));
      console.log('condition: ' + JSON.stringify(condition));

      if (temperature.numericValue.unit === "°C") {
        var temperatureUnit = 0;
      } else {
        var temperatureUnit = 1;
      }
      latestWeather = {
        'TEMPERATURE': temperature.numericValue.value,
        'TEMPERATURE_UNIT': temperatureUnit,
        'CONDITION': weatherConditionToCode(condition.longText.text)
      };
      sendWeather();
    },
    onError: function(err) {
      console.log('Weather unavailable: ' + err.code + ' ' + err.message);
    }
  });
}
function subscribeToBattery() {
  Pebble.subscribeToSource({
    category: 'phone',
    item: 'phone_state',
    properties: ['battery_level'],
    onData: function(update) {
      var here = update.instances[0];
      if (!here) {
        return;
      }

      // Any property can be missing, so check before using it
      var battery_level = here.properties.battery_level;
      if (!battery_level || !battery_level.numericValue) {
        console.log('Battery unavailable');
        return;
      }
      console.log('battery_level: ' + JSON.stringify(battery_level));

      latestBattery = {
        "BATTERY": battery_level.numericValue.value,
      };
      sendBattery();
    },
    onError: function(err) {
      console.log('Weather unavailable: ' + err.code + ' ' + err.message);
    }
  });
}
function subscribeToSun() {
  var sourcePriority = [
    { category: 'astronomy', item: 'sun' },
    { category: 'weather', item: 'sun' },
    { category: 'weather', item: 'location' }
  ];
  var availablePlugins = Pebble.enumeratePlugins() || [];
  var selectedSource = null;

  function hasTimestamp(source, property) {
    var shapes = source.properties && source.properties[property];
    return shapes && shapes.indexOf('timestamp') !== -1;
  }

  for (var i = 0; i < sourcePriority.length && !selectedSource; i++) {
    var preferred = sourcePriority[i];
    for (var j = 0; j < availablePlugins.length && !selectedSource; j++) {
      var plugin = availablePlugins[j];
      if (!plugin.uuid || !plugin.sources) {
        continue;
      }
      for (var k = 0; k < plugin.sources.length; k++) {
        var source = plugin.sources[k];
        if (source.category === preferred.category &&
            source.items && source.items.indexOf(preferred.item) !== -1 &&
            hasTimestamp(source, 'sunrise') &&
            hasTimestamp(source, 'sunset')) {
          selectedSource = {
            plugin: plugin,
            category: preferred.category,
            item: preferred.item
          };
          break;
        }
      }
    }
  }

  if (!selectedSource) {
    console.log('No compatible sun plugin found!');
    return;
  }

  console.log('Using ' + selectedSource.plugin.name + "'s " + selectedSource.category + '/' + selectedSource.item + ' for sun data');
  Pebble.subscribeToSource({
    category: selectedSource.category,
    item: selectedSource.item,
    plugin: selectedSource.plugin.uuid,
    properties: ['sunrise', 'sunset'],
    onData: function(update) {
      var here = update.instances[0];
      var sunrise = here.properties.sunrise
      var sunset = here.properties.sunset
      if (!sunrise || !sunrise.timestamp ||
          !sunset || !sunset.timestamp) {
        console.log('Sun data unavailable');
        return;
      }
      console.log('sunrise: ' + JSON.stringify(sunrise));
      console.log('sunset: ' + JSON.stringify(sunset));

      var sunriseDate = new Date(sunrise.timestamp.value * 1000);
      var sunsetDate = new Date(sunset.timestamp.value * 1000);
      latestSun = {
        'SUNRISE': sunriseDate.getHours() * 100 + sunriseDate.getMinutes(),
        'SUNSET': sunsetDate.getHours() * 100 + sunsetDate.getMinutes()
      };
      sendSun();
    },
    onError: function(err) {
      console.log('Sun data unavailable: ' + err.code + ' ' + err.message);
    }
  });
}
// end plugins

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    Pebble.sendAppMessage({'JSReady': 1}); // Update s_js_ready on watch
    // Get the initial data
    subscribeToWeather();
    subscribeToBattery();
    subscribeToSun();
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
      sendSun();
    }
    // Check if this is a weather refresh request
    if (dict['REQUEST_WEATHER']) {
      sendWeather();
    }
    // Check if this is a battery refresh request
    if (dict['REQUEST_BATTERY']) {
      var batteryToggle = 1;
      localStorage.setItem('phoneBatteryEnabled', batteryToggle);
      sendBattery();
    }
    // Check if this is a battery unsubscribe request
    if (dict['UNSUBSCRIBE_BATTERY']) {
      var batteryToggle = 0;
      localStorage.setItem('phoneBatteryEnabled', batteryToggle);
      stopBattery();
    }
  }
);
