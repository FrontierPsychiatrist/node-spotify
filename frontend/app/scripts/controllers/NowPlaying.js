'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('NowPlayingCtrl', function($scope, socket, events) {
    socket.on(events.now_playing_data_changed, function(image) {
      $scope.image = image;
    });
  });
