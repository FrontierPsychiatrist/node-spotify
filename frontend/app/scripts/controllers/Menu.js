'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('MenuCtrl', function($scope, socket, events) {
    socket.on(events.initial_data, function(playlists) {
      $scope.playlists = playlists;
    });

    $scope.loadPlaylist = function(playlistId) {
      socket.emit(events.playlist_tracks, {id: playlistId});
    };
    socket.emit(events.initial_data);
  });
