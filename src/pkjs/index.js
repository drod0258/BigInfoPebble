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
// date/time constants and conversions
var dayMs = 1000 * 60 * 60 * 24,
  J1970 = 2440588,
  J2000 = 2451545;
function toJulian(date) {
  return date.valueOf() / dayMs - 0.5 + J1970;
}
function fromJulian(j) {
  return new Date((j + 0.5 - J1970) * dayMs);
}
function toDays(date) {
  return toJulian(date) - J2000;
}
// general calculations for position
var e = rad * 23.4397; // obliquity of the Earth
function getRightAscension(l, b) {
  return atan(sin(l) * cos(e) - tan(b) * sin(e), cos(l));
}
function getDeclination(l, b) {
  return asin(sin(b) * cos(e) + cos(b) * sin(e) * sin(l));
}
function getAzimuth(H, phi, dec) {
  return atan(sin(H), cos(H) * sin(phi) - tan(dec) * cos(phi));
}
function getAltitude(H, phi, dec) {
  return asin(sin(phi) * sin(dec) + cos(phi) * cos(dec) * cos(H));
}
function getSiderealTime(d, lw) {
  return rad * (280.16 + 360.9856235 * d) - lw;
}
// general sun calculations
function getSolarMeanAnomaly(d) {
  return rad * (357.5291 + 0.98560028 * d);
}
function getEquationOfCenter(M) {
  return rad * (1.9148 * sin(M) + 0.02 * sin(2 * M) + 0.0003 * sin(3 * M));
}
function getEclipticLongitude(M, C) {
  var P = rad * 102.9372; // perihelion of the Earth
  return M + C + P + PI;
}
function getSunCoords(d) {
  var M = getSolarMeanAnomaly(d),
    C = getEquationOfCenter(M),
    L = getEclipticLongitude(M, C);
  return {
    dec: getDeclination(L, 0),
    ra: getRightAscension(L, 0)
  };
}
var SunCalc = {};
// calculates sun position for a given date and latitude/longitude
SunCalc.getPosition = function (date, lat, lng) {
  var lw  = rad * -lng,
    phi = rad * lat,
    d   = toDays(date),
    c  = getSunCoords(d),
    H  = getSiderealTime(d, lw) - c.ra;
  return {
    azimuth: getAzimuth(H, phi, c.dec),
    altitude: getAltitude(H, phi, c.dec)
  };
};
// sun times configuration (angle, morning name, evening name)
var times = [
  [-0.83, 'sunrise',       'sunset'      ],
  [ -0.3, 'sunriseEnd',    'sunsetStart' ],
  [   -6, 'dawn',          'dusk'        ],
  [  -12, 'nauticalDawn',  'nauticalDusk'],
  [  -18, 'nightEnd',      'night'       ],
  [    6, 'goldenHourEnd', 'goldenHour'  ]
];
// adds a custom time to the times config
SunCalc.addTime = function (angle, riseName, setName) {
  times.push([angle, riseName, setName]);
};
// calculations for sun times
var J0 = 0.0009;
function getJulianCycle(d, lw) {
  return Math.round(d - J0 - lw / (2 * PI));
}
function getApproxTransit(Ht, lw, n) {
  return J0 + (Ht + lw) / (2 * PI) + n;
}
function getSolarTransitJ(ds, M, L) {
  return J2000 + ds + 0.0053 * sin(M) - 0.0069 * sin(2 * L);
}
function getHourAngle(h, phi, d) {
  return acos((sin(h) - sin(phi) * sin(d)) / (cos(phi) * cos(d)));
}
// calculates sun times for a given date and latitude/longitude
SunCalc.getTimes = function (date, lat, lng) {
  var lw  = rad * -lng,
    phi = rad * lat,
    d   = toDays(date),
    n  = getJulianCycle(d, lw),
    ds = getApproxTransit(0, lw, n),
    M = getSolarMeanAnomaly(ds),
    C = getEquationOfCenter(M),
    L = getEclipticLongitude(M, C),
    dec = getDeclination(L, 0),
    Jnoon = getSolarTransitJ(ds, M, L);
  // returns set time for the given sun altitude
  function getSetJ(h) {
    var w = getHourAngle(h, phi, dec),
      a = getApproxTransit(w, lw, n);
    return getSolarTransitJ(a, M, L);
  }
  var result = {
    solarNoon: fromJulian(Jnoon),
    nadir: fromJulian(Jnoon - 0.5)
  };
  var i, len, time, angle, morningName, eveningName, Jset, Jrise;
  for (i = 0, len = times.length; i < len; i += 1) {
    time = times[i];
    Jset = getSetJ(time[0] * rad);
    Jrise = Jnoon - (Jset - Jnoon);
    result[time[1]] = fromJulian(Jrise);
    result[time[2]] = fromJulian(Jset);
  }
  return result;
};
// moon calculations
function getMoonCoords(d) { // geocentric ecliptic coordinates of the moon
  var L = rad * (218.316 + 13.176396 * d), // ecliptic longitude
    M = rad * (134.963 + 13.064993 * d), // mean anomaly
    F = rad * (93.272 + 13.229350 * d),  // mean distance
    l  = L + rad * 6.289 * sin(M), // longitude
    b  = rad * 5.128 * sin(F),     // latitude
    dt = 385001 - 20905 * cos(M);  // distance to the moon in km
  return {
    ra: getRightAscension(l, b),
    dec: getDeclination(l, b),
    dist: dt
  };
}
SunCalc.getMoonIllumination = function (date) {
  var d = toDays(date || new Date()),
    s = getSunCoords(d),
    m = getMoonCoords(d),
    sdist = 149598000, // distance from Earth to Sun in km
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
  var d = new Date();
  var sunTimes = SunCalc.getTimes(d, lat, lon);
  var sunriseint = sunTimes.sunrise.getHours() * 100 + sunTimes.sunrise.getMinutes();
  var sunsetint = sunTimes.sunset.getHours() * 100 + sunTimes.sunset.getMinutes();
  var moonmetrics = SunCalc.getMoonIllumination(d);
  var moonphase = Math.round(moonmetrics.phase * 28);
  console.log("OK API");
  console.log("moonphase " + moonphase);
  console.log("date " + d);
  console.log("lat " + lat);
  console.log("lon " + lon);
  console.log("sunriseint " + sunriseint);
  console.log("sunsetint " + sunsetint);
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
  var dictionary = {
    "BATTERY": batteryLevel
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
  // also send battery level immediately
  sendBatteryLevel(battery, 1);
}
function batteryLevelUnsubscribe(battery) {
  // Stop listening for changes in battery level
  battery.removeEventListener('levelchange', function() {
    sendBatteryLevel(battery, 5);
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
  // Construct Open-Meteo API URL
  var url = 'https://api.open-meteo.com/v1/forecast?' +
      'latitude=' + pos.coords.latitude +
      '&longitude=' + pos.coords.longitude +
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
      var conditions = weatherCodeToCondition(json.current.weather_code);
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
    weatherInfo(mockPosition);
  } else {
    navigator.geolocation.getCurrentPosition(
      weatherInfo,
      locationError,
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
    sunInfo(mockPosition);
  } else {
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
      var lat = dict['Latitude'];
      var lon = dict['Longitude'];
      // If BOTH fields have content and are not just empty strings save to localStorage
      if (lat !== "" && lon !== "" && lat !== undefined && lon !== undefined) {
        var manualCoordinates = 1;
        localStorage.setItem('Latitude', lat);
        localStorage.setItem('Longitude', lon);
      } else {
        var manualCoordinates = 0;
        localStorage.removeItem('Latitude');
        localStorage.removeItem('Longitude');
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
