#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include <memory>
#include <libspotify/api.h>

#include "../node/V8Callable.h"

#include "Track.h"

class Playlist {
friend class NodePlaylist;
friend class PlaylistCallbacks;
friend class PlaylistContainer;
public:
  Playlist(sp_playlist* _playlist, int _id);
  ~Playlist() {
    sp_playlist_release(playlist);
  };
  Playlist(const Playlist& other) : id(other.id), playlist(other.playlist), nodeObject(other.nodeObject) {
    sp_playlist_add_ref(playlist);
  }

  std::vector<std::shared_ptr<Track>> getTracks();
  virtual std::string name();
  std::string link();
private:
  int id;
  sp_playlist* playlist;
  static sp_playlist_callbacks playlistCallbacks;
  V8Callable* nodeObject;
};

#endif
