#include "SearchResult.h"

#include <future>
#include "../../SpotifyService/SpotifyService.h"

#include "../../Application.h"

extern Application* application;

std::vector<std::shared_ptr<Track>> SearchResult::getTracks() {
  std::promise<std::vector<std::shared_ptr<Track>>> pr; 
  auto cb = [&] () {
    std::vector<std::shared_ptr<Track>> tracks(sp_search_num_tracks(search));
    for(int i = 0; i < (int)tracks.size() ; ++i) {
      tracks[i] = std::make_shared<Track>(sp_search_track(search, i));
    }
    pr.set_value(tracks);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  pr.get_future().wait();
  return pr.get_future().get();
}