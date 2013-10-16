//writes the events.h file to a JSON object
var fs = require('fs');

var writeStream = fs.createWriteStream('events.js', {flags: 'w'});

fs.readFile('src/events.h', 'utf8', function(err, data) {
  if (err) throw err;
  var events = {};
  var lines = data.split('\n');
  for(var i = 0; i < lines.length; i++) {
    var line = lines[i];
    var tokens = line.split(' ');
    if(tokens.length == 3) { //only if the line is in the format #define EVENT_NAME "event_name"
      var event = tokens[1].toLowerCase();
      events[event] = event;
    }
  }
  writeStream.write(JSON.stringify(events));
});