#include "PlaylistCallbacks.h"
#include "../spotify/Playlist.h"
#include "../Callback.h"
#include "../events.h"

namespace spotify {

void playlistNameChange(sp_playlist* spPlaylist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  playlist->setName(std::string(sp_playlist_name(spPlaylist)));
}

void playlistStateChanged(sp_playlist* _playlist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  if(sp_playlist_is_loaded(_playlist)) {
    playlist->setName(std::string(sp_playlist_name(_playlist)));
  }
}

}
