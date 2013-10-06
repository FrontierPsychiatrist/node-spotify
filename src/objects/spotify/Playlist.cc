#include "Playlist.h"
#include <future>
#include "../../Application.h"

extern Application* application;

Playlist::Playlist(sp_playlist* _playlist, int _id) : id(_id), playlist(_playlist) {
  sp_playlist_add_ref(playlist);
  if(!sp_playlist_is_loaded(playlist)) {
    name = std::string("Loading...");
  } else {
    name = std::string(sp_playlist_name(playlist));  
  }
};

std::vector<std::shared_ptr<Track>> Playlist::getTracks() {
  std::promise<std::vector<std::shared_ptr<Track>>> pr; 
  auto cb = [&] () {
    std::vector<std::shared_ptr<Track>> tracks(sp_playlist_num_tracks(playlist));
    for(int i = 0; i < (int)tracks.size(); ++i) {
      tracks[i] = std::make_shared<Track>(sp_playlist_track(playlist, i));
    }
    pr.set_value(tracks);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  pr.get_future().wait();
  return pr.get_future().get();
}

sp_playlist_callbacks Playlist::playlistCallbacks;