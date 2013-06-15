'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('MainCtrl', function ($scope, socket, events) {
    $scope.logout = function() {
      socket.emit(events.logout);
    };
    socket.on(events.logged_in, function(data) {
      if(data.loggedIn) {
        socket.emit(events.initial_data);
      } else {

      }
    });
    socket.emit(events.logged_in);
  });
