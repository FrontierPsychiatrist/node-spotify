var express = require('express'),
    app = express();
  var server = require('http').createServer(app);
  var io = require('socket.io').listen(server);
  var spotify = require('../build/Debug/spotify');
  var events = require('../frontend/app/scripts/services/events.js');

server.listen(25000);

app.configure(function() {
	app.use(express.static(__dirname + '/../frontend/dist'));
});

var gSocket;
var playlists;
var loggedIn = false;

//The callback that is called when login is complete
spotify.ready( function() {
    loggedIn = true;
    playlists = spotify.getPlaylists();
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

    socket.on(events.playlist_tracks, function(data) {
        var playlist = playlists[data.id];
        playlist.tracks = playlist.getTracks();
        socket.emit(events.playlist_tracks, playlist);
    });

    socket.on(events.play, function(data) {
        spotify.player.play(data.playlistId, data.trackId);   
    });

    //Client requests initial data
    socket.on(events.initial_data, sendInitialData);
});

function sendInitialData() {
    gSocket.emit(events.initial_data, playlists);
}
