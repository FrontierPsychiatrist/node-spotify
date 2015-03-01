#ifndef _NODE_SEARCH_RESULT_H
#define _NODE_SEARCH_RESULT_H

#include "V8Browseable.h"
#include "../spotify/Search.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeSearch : public V8Browseable<NodeSearch> {
private:
  std::unique_ptr<Search> search;
  std::string searchQuery;
  int trackOffset, albumOffset, artistOffset, playlistOffset;
  int trackLimit, albumLimit, artistLimit, playlistLimit;
  void setupAdditionalMethods();
public:
  NodeSearch(const char* _query);
  NodeSearch(const char* _query, int offset);
  NodeSearch(const char* _query, int offset, int limit);

  static NAN_GETTER(getTrackOffset);
  static NAN_SETTER(setTrackOffset);
  static NAN_GETTER(getAlbumOffset);
  static NAN_SETTER(setAlbumOffset);
  static NAN_GETTER(getArtistOffset);
  static NAN_SETTER(setArtistOffset);
  static NAN_GETTER(getPlaylistOffset);
  static NAN_SETTER(setPlaylistOffset);
  static NAN_GETTER(getTrackLimit);
  static NAN_SETTER(setTrackLimit);
  static NAN_GETTER(getAlbumLimit);
  static NAN_SETTER(setAlbumLimit);
  static NAN_GETTER(getArtistLimit);
  static NAN_SETTER(setArtistLimit);
  static NAN_GETTER(getPlaylistLimit);
  static NAN_SETTER(setPlaylistLimit);
  static NAN_GETTER(didYouMean);
  static NAN_GETTER(getTotalTracks);
  static NAN_GETTER(getNumTracks);
  static NAN_GETTER(getTotalAlbums);
  static NAN_GETTER(getNumAlbums);
  static NAN_GETTER(getTotalArtists);
  static NAN_GETTER(getNumArtists);
  static NAN_GETTER(getTotalPlaylists);
  static NAN_GETTER(getNumPlaylists);
  static NAN_METHOD(New);
  static NAN_METHOD(execute);
  static NAN_METHOD(getTrack);
  static NAN_METHOD(getAlbum);
  static NAN_METHOD(getArtist);
  static NAN_METHOD(getPlaylist);
  static NAN_GETTER(getLink);
  static void init();
};

#endif
