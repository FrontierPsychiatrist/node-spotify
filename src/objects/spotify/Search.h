#ifndef _SEARCH_RESULT_H
#define _SEARCH_RESULT_H

#include <libspotify/api.h>
#include <memory>
#include <vector>
#include <string>

#include "Track.h"

class Search {
friend class NodeSearch;
public:
  Search(sp_search* _search);
  Search(const Search& other) : search(other.search), didYouMeanText(other.didYouMeanText) {
    sp_search_add_ref(search);
  };
  ~Search() {
    sp_search_release(search);
  };
  std::vector<std::shared_ptr<Track>> getTracks();
private:
  sp_search* search;
  std::string didYouMeanText;
  std::string link;
};

#endif