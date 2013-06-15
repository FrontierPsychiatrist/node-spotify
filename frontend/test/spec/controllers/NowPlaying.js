'use strict';

describe('Controller: NowPlayingCtrl', function () {

  // load the controller's module
  beforeEach(module('frontend2App'));

  var NowPlayingCtrl,
    scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    NowPlayingCtrl = $controller('NowPlayingCtrl', {
      $scope: scope
    });
  }));

  /*it('should attach a list of awesomeThings to the scope', function () {
    expect(scope.awesomeThings.length).toBe(3);
  });*/
});
