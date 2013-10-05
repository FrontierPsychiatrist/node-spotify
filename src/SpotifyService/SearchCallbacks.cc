#include "SearchCallbacks.h"

#include "../events.h"
#include "../objects/spotify/SearchResult.h"
#include "../objects/node/NodeSearchResult.h"

void SearchCallbacks::searchComplete(sp_search* spSearch, void* userdata) {
  NodeSearchResult* nodeSearchResult = static_cast<NodeSearchResult*>(userdata);
  auto searchResult = std::make_shared<SearchResult>(spSearch);
  nodeSearchResult->setSearchResult(searchResult);
  nodeSearchResult->call(SEARCH_COMPLETE);
}