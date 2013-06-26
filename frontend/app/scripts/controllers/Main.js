'use strict';
/* jshint camelcase: false */
/* global $ */
angular.module('node-spotify')
  .controller('MainCtrl', function ($scope, socket, events) {
    $scope.logout = function() {
      socket.emit(events.logout);
    };

    var reloginPossible = false;

    $scope.login = function(username, password, rememberMe) {
      if(!rememberMe) {
        rememberMe = false;
      }
      socket.emit(events.login, {username: username, password: password, rememberMe: rememberMe, withRemembered: !rememberMe && reloginPossible});
    };

    socket.on(events.logged_in, function(data) {
      if(data.loggedIn) {
        $('#modal').modal('hide');
        socket.emit(events.initial_data);
      } else {
        $scope.rememberedUser = data.rememberedUser;
        reloginPossible = data.rememberedUser !== undefined;
        $('#modal').modal({keyboard: false});
      }
    });

    socket.emit(events.logged_in);
  });
