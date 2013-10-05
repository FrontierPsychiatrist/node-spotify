#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <libspotify/api.h>
#include <uv.h>
#include <v8.h>
#include <memory>

extern "C" {
  #include "audio/audio.h"
}

class PlaylistContainer;
class SpotifyService;
class NodePlayer;

struct Application {
  sp_session* session;
  audio_fifo_t audio_fifo;
  uv_async_t asyncHandle;
  std::shared_ptr<PlaylistContainer> playlistContainer;
  std::unique_ptr<SpotifyService> spotifyService;
  std::unique_ptr<NodePlayer> nodePlayer;

  //TODO not the best place...
  v8::Persistent<v8::Function> loginCallback;
};

#endif