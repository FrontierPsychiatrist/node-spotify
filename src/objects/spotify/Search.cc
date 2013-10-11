#include "Search.h"

#include <future>

#include "../../Application.h"

extern Application* application;

Search::Search(sp_search* _search) : search(_search) {
  sp_search_add_ref(search);
  didYouMeanText = std::string(sp_search_did_you_mean(search));

  sp_link* spLink = sp_link_create_from_search(search);
  char linkChar[256];
  sp_link_as_string(spLink, linkChar, 256);
  link = std::string(linkChar);
  sp_link_release(spLink);
}

std::vector<std::shared_ptr<Track>> Search::getTracks() {
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