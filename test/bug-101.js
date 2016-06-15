var stream = require('stream');
var ss = new stream.PassThrough();

var baseTest = require('./basetest.js');
var spotify = baseTest.spotify;
spotify.useNodejsAudio(function(err, buffer) {
  console.log('foo', buffer);
  if(err) throw err;
  //ss.write(buffer);
  return true;
});

baseTest.executeTest(tests);

function tests() {
  var track = spotify.sessionUser.starredPlaylist.getTrack(0);
  console.log('playing track ' +  track.name + ' for 2 seconds');
  spotify.player.play(track);

  setTimeout(function() {
    console.log('logging out');
    spotify.logout(function () {
      process.exit();
    });
  }, 2000);
}
