var _spotify = require('./nodespotify');

var beefedupSpotify = function(options) {
  var spotify = _spotify(options);
  spotify.version = '0.4.0';
  spotify.ARTISTBROWSE_FULL = 0;
  spotify.ARTISTBROWSE_NO_TRACKS = 1;
  spotify.ARTISTBROWSE_NO_ALBUMS = 2;
  return spotify;
}

module.exports = beefedupSpotify;