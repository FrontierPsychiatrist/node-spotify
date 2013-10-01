#include "SearchCallbacks.h"

#include "../objects/spotify/SearchResult.h"
#include "../objects/node/NodeSearchResult.h"

void SearchCallbacks::searchComplete(sp_search* spSearch, void* userdata) {
  NodeSearchResult* nodeSearchResult = static_cast<NodeSearchResult*>(userdata);
  std::shared_ptr<SearchResult> searchResult = std::shared_ptr<SearchResult>(new SearchResult(spSearch));
  nodeSearchResult->setSearchResult(searchResult);
  nodeSearchResult->call("SEARCH_COMPLETE");
}