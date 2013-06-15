//Writes the events to a header file for usage in C/C++
var events = require('./frontend/app/scripts/services/events');
var fs = require('fs');

var writeStream = fs.createWriteStream('src/events.h', { flags: 'w'});

function writeEventsToFile(writeStream) {
	writeStream.write('#ifndef _EVENTS_H\n#define _EVENTS_H\n');
	for (var event in events) {
		var define = '#define ' + event.toUpperCase() + ' "' + event + '"\n';
		writeStream.write(define);
	}
	writeStream.write('#endif');
	writeStream.end();
}

writeEventsToFile(writeStream);
