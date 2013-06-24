'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .controller('NowPlayingCtrl', function($scope, socket, events) {
    $scope.image = 'img/noalbumcover.png';

    socket.on(events.now_playing_data_changed, function(data) {
      console.log('Now playing:' + data.trackName);
      $scope.image = 'data:image/jpeg;base64,' + data.image;
    });
  });
