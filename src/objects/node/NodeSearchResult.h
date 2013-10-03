#ifndef _NODE_SEARCH_RESULT_H
#define _NODE_SEARCH_RESULT_H

#include <memory>
#include "NodeWrapped.h"
#include "../spotify/SearchResult.h"

using namespace v8;

class NodeSearchResult : public NodeWrapped<NodeSearchResult> {
private:
  std::shared_ptr<SearchResult> searchResult;
public:
  NodeSearchResult(std::shared_ptr<SearchResult> _searchResult) : searchResult(_searchResult) {};
  NodeSearchResult() {};
  void setSearchResult(std::shared_ptr<SearchResult> _searchResult) {
    searchResult = _searchResult;
  }
  static Handle<Value> didYouMean(const Arguments& args);
  static Handle<Value> getTracks(const Arguments& args);
  static void init();
};

#endif