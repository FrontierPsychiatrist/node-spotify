var spotify = require('../build/Debug/spotify')();
var loginData = require('./loginData.js');

spotify.ready(tests);
spotify.login(loginData.user, loginData.password, false, false);

function tests() {
  /* Playlists */
  var playlists = spotify.getPlaylists();
  console.log('Playlists: ' + playlists.length)
  var starredPlaylist = spotify.getStarred();
  console.log('Starred playlist accessible');
  var firstPlaylist = playlists[0];
  console.log('Playlistname: ' + firstPlaylist.name);
  console.log('Playlistlink: ' + firstPlaylist.link);

  /* Tracks */
  var starredTracks = starredPlaylist.getTracks();
  console.log('Starred tracks: ' + starredTracks.length);
  var firstStarredTrack = starredTracks[0];
  console.log('Trackname: ' + firstStarredTrack.name);
  console.log('Track is starred: ' + firstStarredTrack.starred);
  console.log('Track link: ' + firstStarredTrack.link);
  
  /* Album */ 
  var album = firstStarredTrack.album;
  console.log('Albumname: ' + album.name);
  console.log('Album link: ' + album.link);

  /* Artists */
  var artists = firstStarredTrack.artists;
  console.log('Artists: ' + artists.length);
  var firstArtist = artists[0];
  console.log('Artistname: ' + firstArtist.name);
  console.log('Artist link: ' + firstArtist.link);

  /* Search */
  var search = new spotify.Search('kanje');
  search.execute(function(err, result) {
    console.log('Search: ' + search);
    console.log('Search tracks: ' + search.getTracks().length);
    console.log('Search albums: ' + search.getAlbums().length);
    console.log('Search artists: ' + search.getArtists().length);
    console.log('Search playlists: ' + search.getPlaylists().length);

    spotify.logout();
  });
}