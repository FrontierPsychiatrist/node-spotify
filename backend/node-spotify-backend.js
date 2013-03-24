var server = require('http').createServer(serveStatic),
	fs = require('fs'),
	path = require('path'),
	io = require('socket.io').listen(server),
    spotify = require('../build/Debug/spotify'),
    events = require('../frontend/js/events');

server.listen(25000);
var baseFileUrl = '../frontend/';

function serveStatic(req, res) {
	var filePath = baseFileUrl + req.url;
	
	var contentType = 'text/html';
	var extension = path.extname(filePath);
	switch(extension) {
		case '.js':
			contentType = 'text/javascript';
			break;
		case '.css':
			contentType = 'text/css';
			break;
	}

	fs.exists(filePath, function(exists) {
		if(exists) {
			fs.readFile(filePath, function(error, content) {
				if(error) {
					res.writeHead(500);
					console.log(error);
					res.end();
				} else {
					res.writeHead(200, {'Content-Type': contentType });
					res.end(content, 'utf-8');
				}
			});
		} else {
			res.writeHead(404);
			res.end();
		}
	});
}

var gSocket;
var playlists;
var loggedIn = false;

//The callback that is called when login is complete
spotify.ready( function() {
    loggedIn = true;
    sendInitialData();
});

spotify.playlists.on(events.playlist_renamed, function () {
    gSocket.emit(events.playlist_renamed, this);
})

io.sockets.on('connection', function(socket) {
    gSocket = socket;

    //Client wants to know if the service is logged in
    socket.on(events.logged_in, function() { socket.emit(events.logged_in, {'loggedIn': loggedIn})});

    //Client wants to login
    socket.on(events.login, function(data) {
        spotify.login(data.username, data.password);
    });

    //Client wants to logout
    socket.on(events.logout, function(data) {
        spotify.logout();
    });

    //Client requests initial data
    socket.on(events.initial_data, sendInitialData);
});

function sendInitialData() {
    playlists = spotify.getPlaylists();
    gSocket.emit(events.initial_data, playlists);
}
