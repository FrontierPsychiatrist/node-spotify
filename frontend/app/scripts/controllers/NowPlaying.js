'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('NowPlayingCtrl', function($scope, socket, events) {
    socket.on(events.now_playing_data_changed, function(data) {
      console.log('Now playing:' + data.trackName);
      $scope.image = data.image;
    });

    $scope.pause = function() {
      socket.emit(events.player_pause);
    };

    $scope.resume = function() {
      socket.emit(events.player_resume);
    };

    $scope.back = function() {
      socket.emit(events.player_back);
    };

    $scope.forward = function() {
      socket.emit(events.player_forward);
    };
  });
