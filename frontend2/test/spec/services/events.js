'use strict';

describe('Service: events', function () {

  // load the service's module
  beforeEach(module('node-spotify'));

  // instantiate service
  var events;
  beforeEach(inject(function (_events_) {
    events = _events_;
  }));

  it('new playlist event is defined', function () {
    expect(events.playlist_new).toBe('playlist_new');
  });

});
