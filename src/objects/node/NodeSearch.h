#ifndef _NODE_SEARCH_RESULT_H
#define _NODE_SEARCH_RESULT_H

#include <memory>
#include "NodeWrapped.h"
#include "../spotify/Search.h"

using namespace v8;

class NodeSearch : public NodeWrapped<NodeSearch> {
private:
  std::shared_ptr<Search> search;
public:
  NodeSearch(std::shared_ptr<Search> _search) : search(_search) {};
  NodeSearch() {};
  void setSearch(std::shared_ptr<Search> _search) {
    search = _search;
  }
  static Handle<Value> didYouMean(const Arguments& args);
  static Handle<Value> getTracks(const Arguments& args);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static void init();
};

#endif