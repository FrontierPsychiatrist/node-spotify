'use strict';

describe('Controller: PlaylistCtrl', function () {

  // load the controller's module
  beforeEach(module('frontend2App'));

  var PlaylistCtrl,
    scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    PlaylistCtrl = $controller('PlaylistCtrl', {
      $scope: scope
    });
  }));

  /*it('should attach a list of awesomeThings to the scope', function () {
    expect(scope.awesomeThings.length).toBe(3);
  });*/
});
