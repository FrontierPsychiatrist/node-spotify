'use strict';
/* jshint camelcase: false */
/* global $ */
angular.module('node-spotify')
  .controller('PlayerCtrl', function ($scope, socket, events) {
    var currentSecond = 0;
    var songLengthInSeconds = 239;

    socket.on(events.player_second_in_song, function(data) {
      currentSecond = data;
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

    /* jquery init seekbar */
    var $seekbar = $('#seekbar');
    $seekbar.slider({
        min: 0,
        max: 100,
        step: 0.5,
        value: 0.1,
        layout: 'horizontal',
        formater: function(val) {
          var seconds = val/100 * songLengthInSeconds;
          var secondsInMinute = Math.floor( seconds % 60 );
          if(secondsInMinute < 10) {
            secondsInMinute = '0' + secondsInMinute;
          }
          return Math.floor(seconds / 60) + ':' + secondsInMinute;
        }
      }).on('slideStop', function(event) {
        console.log(event.value);
      });
  });
