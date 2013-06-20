'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('NowPlayingCtrl', function($scope, socket, events) {
    socket.on(events.now_playing_data_changed, function(data) {
      console.log('Now playing:' + data.trackName);
      $scope.image = data.image;
    });
  });
