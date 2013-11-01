#ifndef _NODE_SEARCH_RESULT_H
#define _NODE_SEARCH_RESULT_H

#include <memory>
#include "NodeWrappedWithCallbacks.h"
#include "../spotify/Search.h"

using namespace v8;

class NodeSearch : public NodeWrappedWithCallbacks<NodeSearch> {
private:
  std::shared_ptr<Search> search;
  std::string searchQuery;
  int trackOffset, albumOffset, artistOffset, playlistOffset;
  int trackLimit, albumLimit, artistLimit, playlistLimit;
public:
  NodeSearch(const char* _query);
  NodeSearch(const char* _query, int offset);
  NodeSearch(const char* _query, int offset, int limit);
  void setSearch(std::shared_ptr<Search> _search);

  static Handle<Value> getTrackOffset(Local<String> property, const AccessorInfo& info);
  static void setTrackOffset(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getAlbumOffset(Local<String> property, const AccessorInfo& info);
  static void setAlbumOffset(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getArtistOffset(Local<String> property, const AccessorInfo& info);
  static void setArtistOffset(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getPlaylistOffset(Local<String> property, const AccessorInfo& info);
  static void setPlaylistOffset(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getTrackLimit(Local<String> property, const AccessorInfo& info);
  static void setTrackLimit(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getAlbumLimit(Local<String> property, const AccessorInfo& info);
  static void setAlbumLimit(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getArtistLimit(Local<String> property, const AccessorInfo& info);
  static void setArtistLimit(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> getPlaylistLimit(Local<String> property, const AccessorInfo& info);
  static void setPlaylistLimit(Local<String> property, Local<Value> value,  const AccessorInfo& info);
  static Handle<Value> didYouMean(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalAlbums(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalPlaylists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> New(const Arguments& args);
  static Handle<Value> execute(const Arguments& args);
  static Handle<Value> getTracks(const Arguments& args);
  static Handle<Value> getAlbums(const Arguments& args);
  static Handle<Value> getArtists(const Arguments& args);
  static Handle<Value> getPlaylists(const Arguments& args);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Function> getConstructor();
  static void init();
};

#endif