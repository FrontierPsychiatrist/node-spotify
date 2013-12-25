var assert = require('assert');
var baseTest = require('./basetest.js');
var spotify = baseTest.spotify;

baseTest.executeTest(tests);

var newPlaylist;
var newIndex;
var playlistCountOriginal;
spotify.playlists.on('playlist_added', function(err, playlist, index) {
  newPlaylist = playlist;
  newIndex = index;
});

function tests() {
  console.log('Starting test');
  playlistCountOriginal = spotify.getPlaylists().length;
  var pl = spotify.addPlaylist('My test playlist');
  setTimeout(playlistAdded, 1000);
}

function playlistAdded() {
  assert(newPlaylist !== undefined);
  console.log('New playlist was added');
  newPlaylist.delete();
  setTimeout(playlistDeleted, 1000);
}

function playlistDeleted() {
  assert(spotify.getPlaylists().length === playlistCountOriginal);
  console.log('Playlist was deleted');
  spotify.logout();
  baseTest.quit();
}