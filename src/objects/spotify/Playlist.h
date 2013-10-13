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
friend class PlaylistContainer;
friend class PlaylistCallbacks;
public:
  Playlist(sp_playlist* _playlist, int _id);
  ~Playlist() {
    sp_playlist_release(playlist);
  };
  Playlist(const Playlist& other) : id(other.id), tracks(other.tracks), playlist(other.playlist), name(other.name), link(other.link) {
    sp_playlist_add_ref(playlist);
  }

  std::vector<std::shared_ptr<Track>> getTracks();
private:
  int id;
  std::vector<std::shared_ptr<Track>> tracks;
  sp_playlist* playlist;
  std::string name;
  std::string link;
  static sp_playlist_callbacks playlistCallbacks;
  V8Callable* nodeObject;//TODO: what if this is deleted?
};

#endif
