// Import the Clay package
var Clay = require('@rebble/clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);

// plugin variables
var latestWeather = null;
var latestBattery = null;
var latestSun = null;
var latestMoon = null;

// plugin helper functions
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

function hasShape(source, property, shape) {
  var shapes = source.properties && source.properties[property];
  return shapes && shapes.indexOf(shape) !== -1;
}

function illuminationFraction(illumination) {
  var value = illumination.value;
  var min = illumination.min;
  var max = illumination.max;
  if (max > min) {
    value = (value - min) / (max - min);
  } else if (illumination.unit === '%' || value > 1) {
    value /= 100;
  }
  if (value < 0 || value > 1) {
    return null;
  }
  return value;
}

function phaseDirectionFromText(text) {
  if (typeof text !== 'string') {
    return null;
  }
  var phase = text.toLowerCase().replace(/[_-]+/g, ' ');
  if (phase.indexOf('new') !== -1) return 'new';
  if (phase.indexOf('full') !== -1) return 'full';
  if (phase.indexOf('waxing') !== -1 ||
      phase.indexOf('first') !== -1) return 'waxing';
  if (phase.indexOf('waning') !== -1 ||
      phase.indexOf('third') !== -1 ||
      phase.indexOf('last') !== -1) return 'waning';
  return null;
}

function phaseIndexFromIllumination(illumination, phaseText) {
  var direction = phaseDirectionFromText(phaseText.text);
  var fraction = illuminationFraction(illumination);

  // New and full are the endpoints where waxing/waning is ambiguous.
  if (fraction === 0 || direction === 'new') return 0;
  if (fraction === 1 || direction === 'full') return 14;
  if (fraction === null || direction === null) {
    return null;
  }

  // Illumination is symmetric, so phase text supplies the missing direction.
  var phase = Math.acos(1 - 2 * fraction) / (2 * Math.PI);
  if (direction === 'waning') {
    phase = 1 - phase;
  }
  return Math.max(0, Math.min(28, Math.round(phase * 28)));
}

// send data functions
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
function sendMoon() {
  if (!latestMoon) {
    return;
  }
  Pebble.sendAppMessage(latestMoon,
    function(e) {
      console.log('Moon info sent!');
    },
    function(e) {
      console.log('Error sending Moon info!');
    }
  );
}

// plugin subscription functions
function subscribeToWeather() {
  Pebble.subscribeToSource({
    category: 'weather',
    item: 'location',
    properties: ['temperature', 'condition_code'],
    onData: function(update) {
      var here = update.instances[0];
      if (!here) {
        console.log('Weather unavailable');
        if (!latestWeather || 
            latestWeather.TEMPERATURE !== 200 ||
            latestWeather.CONDITION !== 8) {
          latestWeather = {
            'TEMPERATURE': 200,
            'TEMPERATURE_UNIT': 0,
            'CONDITION': 8
          };
          sendWeather();
        }
        return;
      }

      // Any property can be missing, so check before using it
      var temperature = here.properties.temperature;
      var condition = here.properties.condition_code;
      if (!temperature || !temperature.numericValue ||
          !condition || !condition.longText) {
        console.log('Weather unavailable');
        if (!latestWeather || 
            latestWeather.TEMPERATURE !== 200 ||
            latestWeather.CONDITION !== 8) {
          latestWeather = {
            'TEMPERATURE': 200,
            'TEMPERATURE_UNIT': 0,
            'CONDITION': 8
          };
          sendWeather();
        }
        return;
      }
      // useful debug logs showing source data format
      //console.log('temperature: ' + JSON.stringify(temperature));
      //console.log('condition: ' + JSON.stringify(condition));

      if (temperature.numericValue.unit === "°C") {
        var temperatureUnit = 0;
      } else {
        var temperatureUnit = 1;
      }
      var weatherCode = weatherConditionToCode(condition.longText.text);
      if (!latestWeather || 
          latestWeather.TEMPERATURE !== temperature.numericValue.value ||
          latestWeather.TEMPERATURE_UNIT !== temperatureUnit ||
          latestWeather.CONDITION !== weatherCode) {
        latestWeather = {
          'TEMPERATURE': temperature.numericValue.value,
          'TEMPERATURE_UNIT': temperatureUnit,
          'CONDITION': weatherCode
        };
        console.log(latestWeather);
        sendWeather();
      }
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
        console.log('Battery unavailable');
        return;
      }

      // Any property can be missing, so check before using it
      var battery_level = here.properties.battery_level;
      if (!battery_level || !battery_level.numericValue) {
        console.log('Battery unavailable');
        return;
      }
      // useful debug logs showing source data format
      //console.log('battery_level: ' + JSON.stringify(battery_level));

      if (!latestBattery || latestBattery.BATTERY !== battery_level.numericValue.value) {
        latestBattery = { 'BATTERY': battery_level.numericValue.value };
        console.log(latestBattery);
        sendBattery();
      }
    },
    onError: function(err) {
      console.log('Weather unavailable: ' + err.code + ' ' + err.message);
    }
  });
}

function subscribeToSun() {
  // find a plugin that provides sun timestamps
  var sourcePriority = [
    { category: 'astronomy', item: 'sun' },
    { category: 'weather', item: 'sun' },
    { category: 'weather', item: 'location' }
  ];
  var availablePlugins = Pebble.enumeratePlugins() || [];
  var selectedSource = null;
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
            hasShape(source, 'sunrise', 'timestamp') &&
            hasShape(source, 'sunset', 'timestamp')) {
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
    if (!latestSun || 
        latestSun.SUNRISE !== 0 ||
        latestSun.SUNSET !== 2400) {
      latestSun = {
        'SUNRISE': 0,
        'SUNSET': 2400
      };
    }
    sendSun();
    return;
  }
  console.log('Using ' + selectedSource.plugin.name + "'s " + selectedSource.category + '/' + selectedSource.item + ' for sun data');
  
  // subscribe to selected sun plugin
  Pebble.subscribeToSource({
    category: selectedSource.category,
    item: selectedSource.item,
    plugin: selectedSource.plugin.uuid,
    properties: ['sunrise', 'sunset'],
    onData: function(update) {
      var here = update.instances[0];
      if (!here) {
        console.log('Sun data unavailable');
        if (!latestSun || 
            latestSun.SUNRISE !== 0 ||
            latestSun.SUNSET !== 2400) {
          latestSun = {
            'SUNRISE': 0,
            'SUNSET': 2400
          };
        }
        sendSun();
        return;
      }

      // Any property can be missing, so check before using it
      var sunrise = here.properties.sunrise
      var sunset = here.properties.sunset
      if (!sunrise || !sunrise.timestamp ||
          !sunset || !sunset.timestamp) {
        console.log('Sun data unavailable');
        if (!latestSun || 
            latestSun.SUNRISE !== 0 ||
            latestSun.SUNSET !== 2400) {
          latestSun = {
            'SUNRISE': 0,
            'SUNSET': 2400
          };
        }
        sendSun();
        return;
      }
      // useful debug logs showing source data format
      //console.log('sunrise: ' + JSON.stringify(sunrise));
      //console.log('sunset: ' + JSON.stringify(sunset));

      // convert timestamps to HHMM format
      // sunrise
      var sunriseDate = new Date(sunrise.timestamp.value * 1000);
      var sunriseHour = sunriseDate.getHours();
      var sunriseMinute = sunriseDate.getMinutes();
      var sunriseTime = sunriseHour * 100 + sunriseMinute;
      // sunset
      var sunsetDate = new Date(sunset.timestamp.value * 1000);
      var sunsetHour = sunsetDate.getHours();
      var sunsetMinute = sunsetDate.getMinutes();
      var sunsetTime = sunsetHour * 100 + sunsetMinute;

      if (!latestSun || 
          latestSun.SUNRISE !== sunriseTime ||
          latestSun.SUNSET !== sunsetTime) {
        latestSun = {
          'SUNRISE': sunriseTime,
          'SUNSET': sunsetTime
        };
        console.log(latestSun);
        sendSun();
      }
    },
    onError: function(err) {
      console.log('Sun data unavailable: ' + err.code + ' ' + err.message);
    }
  });
}

function subscribeToMoon() {
  // find a plugin that provides moon illumination and phase
  var sourcePriority = [
    { category: 'astronomy', item: 'moon' },
    { category: 'weather', item: 'moon' }
  ];
  var availablePlugins = Pebble.enumeratePlugins() || [];
  var selectedSource = null;
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
            hasShape(source, 'illumination', 'numericValue') &&
            hasShape(source, 'phase', 'shortText')) {
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
    console.log('No compatible moon plugin found!');
    return;
  }
  console.log('Using ' + selectedSource.plugin.name + "'s " + selectedSource.category + '/' + selectedSource.item + ' for moon data');
  
  // subscribe to selected moon plugin
  Pebble.subscribeToSource({
    category: selectedSource.category,
    item: selectedSource.item,
    plugin: selectedSource.plugin.uuid,
    properties: ['illumination', 'phase'],
    onData: function(update) {
      var here = update.instances[0];
      if (!here || !here.properties) {
        console.log('Moon phase unavailable');
        return;
      }

      // Any property can be missing, so check before using it
      var phase = here.properties.phase;
      var illumination = here.properties.illumination;
      if (!phase || !phase.longText ||
          !illumination || !illumination.numericValue) {
        console.log('Moon phase unavailable');
        return;
      }
      // useful debug logs showing source data format
      //console.log('moon phase: ' + JSON.stringify(phase));
      //console.log('moon illumination: ' + JSON.stringify(illumination));

      // convert phase to an index (0-28; 0 = new, 14 = full)
      var moonPhase = phaseIndexFromIllumination(illumination.numericValue, phase.longText);
      if (moonPhase === null) {
        console.log('Moon phase unavailable');
        return;
      }
      if (!latestMoon || latestMoon.MOONPHASE !== moonPhase) {
        latestMoon = { 'MOONPHASE': moonPhase };
        console.log(latestMoon);
        sendMoon();
      }
    },
    onError: function(err) {
      console.log('Moon data unavailable: ' + err.code + ' ' + err.message);
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
    subscribeToMoon();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    var dict = e.payload;

    // Check if this is a sun info refresh request
    if (dict['REQUEST_SUN']) {
      sendSun();
    }
    // Check if this is a moon info refresh request
    if (dict['REQUEST_MOON']) {
      sendMoon();
    }
    // Check if this is a weather refresh request
    if (dict['REQUEST_WEATHER']) {
      sendWeather();
    }
    // Check if this is a battery refresh request
    if (dict['REQUEST_BATTERY']) {
      sendBattery();
    }
  }
);
