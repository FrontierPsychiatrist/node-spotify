#ifndef _NODE_SPOTIFY_H
#define _NODE_SPOTIFY_H

#include "NodeWrapped.h"
#include "../spotify/Spotify.h"

#include <v8.h>
#include <memory>

using namespace v8;

class NodeSpotify : public NodeWrapped<NodeSpotify> {
public:
  NodeSpotify(Handle<Object> option);
  ~NodeSpotify();
  static NAN_METHOD(login);
  static NAN_METHOD(logout);
  static NAN_GETTER(getPlaylistContainer);
  static NAN_GETTER(getRememberedUser);
  static NAN_GETTER(getSessionUser);
  static NAN_GETTER(getSessionUserCountry);
  static NAN_METHOD(createFromLink);
  static NAN_GETTER(getConstants);
#ifdef NODE_SPOTIFY_NATIVE_SOUND
  static NAN_METHOD(useNativeAudio);
#endif
  static NAN_METHOD(useNodejsAudio);
  static NAN_METHOD(on);
  static void init();
private:
  std::unique_ptr<Spotify> spotify;
};

#endif
