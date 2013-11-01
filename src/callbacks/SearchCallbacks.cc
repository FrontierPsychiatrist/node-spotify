#include "SearchCallbacks.h"

#include "../events.h"
#include "../objects/spotify/Search.h"
#include "../objects/node/NodeSearch.h"

void SearchCallbacks::searchComplete(sp_search* spSearch, void* userdata) {
  NodeSearch* nodeSearch = static_cast<NodeSearch*>(userdata);
  auto search = std::make_shared<Search>(spSearch);
  nodeSearch->setSearch(search);
  nodeSearch->call(SEARCH_COMPLETE);
}