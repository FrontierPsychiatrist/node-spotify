var express = require('express'),
    app = express();
  var server = require('http').createServer(app);
  var io = require('socket.io').listen(server);
  var spotify = require('../build/Release/spotify');
  var events = require('../frontend/app/scripts/services/events.js');

server.listen(25000);

app.configure(function() {
	app.use(express.static(__dirname + '/../frontend/dist'));
});

function Player(socket, spotify, events) {
    var socket = socket;
    var spotify = spotify;
    var currentTrack = {};
    var queue = []; //TODO: this does not work when the playlist gets updated while playing

    spotify.player.on(events.player_end_of_track, function() {
        playNextTrack();
    });

    spotify.player.on(events.player_second_in_song, function() {
        socket.emit(events.player_second_in_song, this.currentSecond);
    });

    socket.on(events.player_seek, function(second) {
        spotify.player.seek(second);
    });

    socket.on(events.play, function(data) {
        queue = playlists[data.playlistId].getTracks();
        currentTrack = queue.filter( 
            function(track) { 
                return track.id == data.trackId 
            })[0];;
        internPlay();
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
       playNextTrack();
    });

    function internPlay() {
        if(currentTrack) {
            spotify.player.play(currentTrack); 
            socket.emit(events.now_playing_data_changed, currentTrack);
            socket.emit(events.now_playing_picture_changed, currentTrack.album.getCoverBase64());
        }
    }

    function playNextTrack() {
        var currentTrackPosition = queue.indexOf( currentTrack );
        if(currentTrackPosition < queue.length - 1) {
            currentTrack = queue[currentTrackPosition + 1];
            internPlay();
        }
    }
}

//variables for ALL sockets
var playlists;
var loggedIn = false;


io.sockets.on('connection', function(socket) {
    //variables per socket
    var player = new Player(socket, spotify, events);
    var displayedPlaylist;    
    
    //Client wants to know if the service is logged in
    socket.on(events.logged_in, function() {
        socket.emit(events.logged_in, {'loggedIn': loggedIn, 'rememberedUser': spotify.rememberedUser()})
    });

    //Client wants to login
    socket.on(events.login, function(data) {
        spotify.login(data.username, data.password, data.rememberMe, data.withRemembered);
    });

    spotify.ready( function() {
        loggedIn = true;
        playlists = spotify.getPlaylists();
        sendInitialData();
    });

    spotify.playlists.on(events.playlist_renamed, function () {
        socket.emit(events.playlist_renamed, this);
    });

    //Client wants to logout
    socket.on(events.logout, function(data) {
        spotify.logout();
    });

    socket.on(events.playlist_tracks, function(data) {
        function generateTrackIds(tracks) {
            for(var i=0;i<tracks.length;i++) {
                tracks[i].id = i;
            }
        }

        function loadAttachAndSendTracks(playlist) {
            playlist.tracks = playlist.getTracks();
            generateTrackIds(playlist.tracks);
            socket.emit(events.playlist_tracks, playlist);
        }

        if(displayedPlaylist) {
            displayedPlaylist.off(events.playlist_tracks_changed);
        }

        displayedPlaylist = playlists[data.id];
        //If tracks change for the currently displayed playlist, update them
        displayedPlaylist.on(events.playlist_tracks_changed, function() {
            loadAttachAndSendTracks(displayedPlaylist);
        });

        loadAttachAndSendTracks(displayedPlaylist);
        //socket.emit(events.playlist_image, displayedPlaylist.getImageBase64());
    });

    socket.on(events.track_set_starred, function(data){
        playlists[data.playlistId].getTracks()[data.trackId].starred = data.starred;
    });

    //Client requests initial data
    socket.on(events.initial_data, sendInitialData);

    function sendInitialData() {
        socket.emit(events.initial_data, playlists);
    }
});
