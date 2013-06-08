'use strict';

angular.module('node-spotify')
  .controller('MenuCtrl', function($scope, Socket, events) {
  	Socket.on(events.initial_data, function(playlists) {
  		$scope.playlists = playlists;
  	});

  	$scope.loadPlaylist = function(playlistId) {
  		Socket.emit(events.playlist_tracks, {id: playlistId});
  	}
  	Socket.emit(events.initial_data);
  });
