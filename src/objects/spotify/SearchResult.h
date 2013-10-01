#ifndef _SEARCH_RESULT_H
#define _SEARCH_RESULT_H

#include <libspotify/api.h>

class SearchResult {
friend class NodeSearchResult;
public:
  SearchResult(sp_search* _search) : search(_search) {
    sp_search_add_ref(search);
    didYouMeanText = sp_search_did_you_mean(search);
  };
  SearchResult(const SearchResult& other) : search(other.search), didYouMeanText(other.didYouMeanText) {
    sp_search_add_ref(search);
  };
  ~SearchResult() {
    sp_search_release(search);
  };
private:
	sp_search* search;
	const char* didYouMeanText;
};

#endif