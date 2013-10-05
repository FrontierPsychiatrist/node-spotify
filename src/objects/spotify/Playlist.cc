#include "Playlist.h"
#include <future>
#include "../../SpotifyService/SpotifyService.h"
#include "../../Application.h"

extern Application* application;

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