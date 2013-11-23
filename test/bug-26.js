var spotify = require('../build/Debug/spotify')({appkeyFile: '../spotify_appkey.key'});
var loginData = require('./loginData.js');

spotify.ready(test);
spotify.login(loginData.user, loginData.password, false, false);

function test() {
  var track = spotify.createFromLink('spotify:track:6koWevx9MqN6efQ6qreIbm');
  spotify.player.play(track);
}