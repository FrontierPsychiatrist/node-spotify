'use strict';
/* jshint camelcase: false */
/* global $ */
angular.module('node-spotify')
  .controller('MainCtrl', function ($scope, socket, events) {
    $scope.logout = function() {
      socket.emit(events.logout);
    };
    $scope.login = function(username, password) {
      socket.emit(events.login, {username: username, password: password});
    };
    socket.on(events.logged_in, function(data) {
      if(data.loggedIn) {
        $('#modal').modal('hide');
        socket.emit(events.initial_data);
      } else {
        $('#modal').modal({keyboard: false});
      }
    });
    socket.emit(events.logged_in);
  });
