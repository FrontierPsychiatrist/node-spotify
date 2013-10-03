#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H

#include <libspotify/api.h>
#include <vector>
#include <memory>

#include "Playlist.h"

class PlaylistContainer {
public:
  PlaylistContainer(sp_playlistcontainer* _playlistContainer) : playlistContainer(_playlistContainer) {};
  void loadPlaylists();
  std::vector<std::shared_ptr<Playlist>> getPlaylists() { return playlists; };
private:
  sp_playlistcontainer* playlistContainer;
  std::vector<std::shared_ptr<Playlist>> playlists;
};

#endif
