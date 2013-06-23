'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('PlayerCtrl', function ($scope, socket, events) {
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
