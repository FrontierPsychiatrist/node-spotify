'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('PlaylistCtrl', function($scope, socket, events) {
    $scope.playlist = {name: 'Choose a playlist'};

    socket.on(events.playlist_tracks, function(data) {
      $scope.playlist = {name: data.name, tracks: data.tracks};
    });

    $scope.play = function(playlistId, trackId) {
      socket.emit(events.play, {playlistId: playlistId, trackId: trackId});
      console.log('Attempting to play track ' + trackId + ' in playlist ' + playlistId);
    };
  });
