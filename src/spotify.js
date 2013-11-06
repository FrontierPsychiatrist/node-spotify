var _spotify = require('./nodespotify');

var beefedupSpotify = function(options) {
  var spotify = _spotify(options);
  spotify.version = '0.4.0';
  return spotify;
}

module.exports = beefedupSpotify;