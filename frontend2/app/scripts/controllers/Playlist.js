'use strict';

angular.module('node-spotify')
  .controller('PlaylistCtrl', function($scope, Socket, events) {
  	$scope.playlist = {name: 'Choose a playlist'};
  	
  	Socket.on(events.playlist_tracks, function(data) {
  		$scope.playlist = {name: data.name, tracks: data.tracks};
  	});
  });
