#ifndef _NODE_SPOTIFY_H
#define _NODE_SPOTIFY_H

#include "NodeWrapped.h"
#include "../spotify/Spotify.h"
#include <v8.h>

using namespace v8;

class NodeSpotify : public NodeWrapped<NodeSpotify> {
public:
  NodeSpotify(Handle<Object> option);
  ~NodeSpotify() {};
  static Handle<Value> login(const Arguments& args);
  static Handle<Value> logout(const Arguments& args);
  static Handle<Value> ready(const Arguments& args);
  static Handle<Value> getPlaylists(const Arguments& args);
  static Handle<Value> getStarred(const Arguments& args);
  static Handle<Value> rememberedUser(const Arguments& args);
  static void init();
private:
  std::unique_ptr<Spotify> spotify;
};

#endif