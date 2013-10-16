#include "Playlist.h"
#include <future>
#include "../../Application.h"

extern Application* application;

Playlist::Playlist(sp_playlist* _playlist, int _id) : id(_id), playlist(_playlist), nodeObject(nullptr) {
  sp_playlist_add_ref(playlist);
};

std::string Playlist::name() {
  std::string name;
  if(sp_playlist_is_loaded(playlist)) {
    name = std::string(sp_playlist_name(playlist));
  } else {
    name = std::string("Loading...");
  }
  return name;
}

std::string Playlist::link() {
  std::string link;
  if(sp_playlist_is_loaded(playlist)) {
    sp_link* spLink = sp_link_create_from_playlist(playlist);
    if(spLink != nullptr) {
      char linkChar[256];
      sp_link_as_string(spLink, linkChar, 256);
      link = std::string(linkChar);
      sp_link_release(spLink);
    }
  }
  return link;
}

std::vector<std::shared_ptr<Track>> Playlist::getTracks() {
  std::vector<std::shared_ptr<Track>> tracks(sp_playlist_num_tracks(playlist));
  for(int i = 0; i < (int)tracks.size(); ++i) {
    tracks[i] = std::make_shared<Track>(sp_playlist_track(playlist, i));
  }
  return tracks;
}

sp_playlist_callbacks Playlist::playlistCallbacks;