#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include <memory>
#include <libspotify/api.h>

#include "Track.h"

class Playlist {
friend class NodePlaylist;
friend class PlaylistContainer;
friend class PlaylistCallbacks;
public:
  Playlist(sp_playlist* _playlist, int _id) : id(_id), playlist(_playlist) {
    sp_playlist_add_ref(playlist);
    if(!sp_playlist_is_loaded(playlist)) {
      name = std::string("Loading...");
    } else {
      name = std::string(sp_playlist_name(playlist));  
    }
  };
  ~Playlist() {
    sp_playlist_release(playlist);
  };
  Playlist(const Playlist& other) : id(other.id), tracks(other.tracks), playlist(other.playlist), name(other.name) {
    sp_playlist_add_ref(playlist);
  }

  std::vector<std::shared_ptr<Track>> getTracks();
private:
  int id;
  std::vector<std::shared_ptr<Track>> tracks;
  sp_playlist* playlist;
  std::string name;
  static sp_playlist_callbacks playlistCallbacks;
};

#endif
