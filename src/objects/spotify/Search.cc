#include "Search.h"

#include "../../Application.h"

extern Application* application;

Search::Search(sp_search* _search) : search(_search) {
  sp_search_add_ref(search);
  totalTracks = sp_search_total_tracks(search);
  totalAlbums = sp_search_total_albums(search);
  totalArtists = sp_search_total_artists(search);
  totalPlaylists = sp_search_total_playlists(search);
}

std::string Search::link() {
  std::string link;
  if(sp_search_is_loaded(search)) {
    sp_link* spLink = sp_link_create_from_search(search);
    char linkChar[256];
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

std::string Search::didYouMeanText() {
  std::string didYouMeanText;
  if(sp_search_is_loaded(search)) {
    didYouMeanText = std::string(sp_search_did_you_mean(search));
  }
  return didYouMeanText;
}

std::vector<std::shared_ptr<Track>> Search::getTracks() {
  std::vector<std::shared_ptr<Track>> tracks(sp_search_num_tracks(search));
  for(int i = 0; i < (int)tracks.size() ; ++i) {
    tracks[i] = std::make_shared<Track>(sp_search_track(search, i));
  }
  return tracks;
}

std::vector<std::shared_ptr<Album>> Search::getAlbums() {
  std::vector<std::shared_ptr<Album>> albums(sp_search_num_albums(search));
  for(int i = 0; i < (int)albums.size() ; ++i) {
    albums[i] = std::make_shared<Album>(sp_search_album(search, i));
  }
  return albums;
}

std::vector<std::shared_ptr<Artist>> Search::getArtists() {
  std::vector<std::shared_ptr<Artist>> artists(sp_search_num_artists(search));
  for(int i = 0; i < (int)artists.size() ; ++i) {
    artists[i] = std::make_shared<Artist>(sp_search_artist(search, i));
  }
  return artists;
}

std::vector<std::shared_ptr<Playlist>> Search::getPlaylists() {
  std::vector<std::shared_ptr<Playlist>> playlists(sp_search_num_playlists(search));
  for(int i = 0; i < (int)playlists.size() ; ++i) {
    playlists[i] = std::make_shared<Playlist>(sp_search_playlist(search, i), -1);
  }
  return playlists;
}

void Search::execute(std::string query, int trackOffset, int trackLimit,
    int albumOffset, int albumLimit,
    int artistOffset, int artistLimit,
    int playlistOffset, int playlistLimit) {
  //TODO: implement search here
}