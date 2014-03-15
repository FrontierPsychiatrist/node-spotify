var assert = require('assert');
var baseTest = require('./basetest.js');
var spotify = baseTest.spotify;

baseTest.executeTest(tests);

var playlistContainer;
var newPlaylist;
var newIndex;
var playlistCountOriginal;

function tests() {
  console.log('Starting test');
  playlistContainer = spotify.playlistContainer;
  playlistContainer.on('playlist_added', function(err, playlist, index) {
    newPlaylist = playlist;
    newIndex = index;
  });
  playlistCountOriginal = playlistContainer.getPlaylists().length;
  var pl = playlistContainer.addPlaylist('My test playlist');
  setTimeout(playlistAdded, 1000);
}

function playlistAdded() {
  assert(newPlaylist !== undefined);
  console.log('New playlist was added');
  playlistContainer.deletePlaylist(newIndex);
  setTimeout(playlistDeleted, 1000);
}

function playlistDeleted() {
  assert(playlistContainer.getPlaylists().length === playlistCountOriginal);
  console.log('Playlist was deleted');
  spotify.logout(function () {
    process.exit();
  });
}
