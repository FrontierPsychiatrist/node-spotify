#include "Playlist.h"
#include <future>
#include "../../SpotifyService/SpotifyService.h"
#include "../../Application.h"

extern Application* application;

std::vector<std::shared_ptr<Track>> Playlist::getTracks() {
  std::promise<std::vector<std::shared_ptr<Track>>> pr; 
  auto cb = [&] () {
    std::vector<std::shared_ptr<Track>> tracks;
    for(int i = 0; i < sp_playlist_num_tracks(playlist); ++i) {
      std::shared_ptr<Track> trackPointer(new Track(sp_playlist_track(playlist, i)));
      tracks.push_back(trackPointer);
    }
    pr.set_value(tracks);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  pr.get_future().wait();
  return pr.get_future().get();
}

sp_playlist_callbacks Playlist::playlistCallbacks;