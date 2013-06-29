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
var currentlyPlayingTrack;
var currentlyPlayingPlaylist;
var currentlyDisplayedPlaylist;

//The callback that is called when login is complete
spotify.ready( function() {
    loggedIn = true;
    playlists = spotify.getPlaylists();
    sendInitialData();
});

spotify.playlists.on(events.playlist_renamed, function () {
    gSocket.emit(events.playlist_renamed, this);
});

io.sockets.on('connection', function(socket) {
    gSocket = socket;

    //Client wants to know if the service is logged in
    socket.on(events.logged_in, function() {
        socket.emit(events.logged_in, {'loggedIn': loggedIn, 'rememberedUser': spotify.rememberedUser()})
    });

    //Client wants to login
    socket.on(events.login, function(data) {
        spotify.login(data.username, data.password, data.rememberMe, data.withRemembered);
    });

    //Client wants to logout
    socket.on(events.logout, function(data) {
        spotify.logout();
    });

    socket.on(events.playlist_tracks, function(data) {
        if(currentlyDisplayedPlaylist) {
            currentlyDisplayedPlaylist.off(events.playlist_tracks_changed);
        }
        currentlyDisplayedPlaylist = playlists[data.id];
        //If tracks change for the currently displayed playlist, update them
        currentlyDisplayedPlaylist.on(events.playlist_tracks_changed, function() {
            currentlyDisplayedPlaylist.tracks = currentlyDisplayedPlaylist.getTracks();
            socket.emit(events.playlist_tracks, currentlyDisplayedPlaylist);
        });
        currentlyDisplayedPlaylist.tracks = currentlyDisplayedPlaylist.getTracks();
        socket.emit(events.playlist_tracks, currentlyDisplayedPlaylist);
    });

    socket.on(events.play, function(data) {
        currentlyPlayingTrack = data.trackId;
        currentlyPlayingPlaylist = playlists[data.playlistId];
        play();
    });

    socket.on(events.player_pause, function() {
        spotify.player.pause();
    });

    socket.on(events.player_resume, function() {
        spotify.player.resume();
    });

    socket.on(events.player_back, function() {
        //spotify.player.pause();
    });

    socket.on(events.player_forward, function() {
       currentlyPlayingTrack++;
       play();
    });

    spotify.player.on(events.player_second_in_song, function() {
        socket.emit(events.player_second_in_song, this.currentSecond);
    });

    spotify.player.on(events.player_end_of_track, function() {
        currentlyPlayingTrack++;
        play();
    });

    socket.on(events.track_set_starred, function(data){
        playlists[data.playlistId].getTracks()[data.trackId].starred = data.starred;
    });

    //Client requests initial data
    socket.on(events.initial_data, sendInitialData);

    function play() {
        spotify.player.play(currentlyPlayingPlaylist.getTracks()[currentlyPlayingTrack]); 
        socket.emit(events.now_playing_data_changed, spotify.player.getCurrentlyPlayingData());
        socket.emit(events.now_playing_picture_changed, currentlyPlayingPlaylist.getTracks()[currentlyPlayingTrack].album.getCoverBase64());
    }
});

function sendInitialData() {
    gSocket.emit(events.initial_data, playlists);
}
