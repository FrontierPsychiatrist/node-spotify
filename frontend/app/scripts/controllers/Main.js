'use strict';

angular.module('node-spotify')
  .controller('MainCtrl', ['$scope', function ($scope) {
    $scope.logout = function() {
      console.log('Logging out');
    };
  }]);
