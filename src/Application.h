#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <libspotify/api.h>
#include <v8.h>
#include <memory>
#include "SpotifyService/SpotifyService.h"
#include "objects/spotify/PlaylistContainer.h"

extern "C" {
  #include "audio/audio.h"
}

struct Application {
  sp_session* session;
  audio_fifo_t audio_fifo;
  std::shared_ptr<PlaylistContainer> playlistContainer;
  std::unique_ptr<SpotifyService> spotifyService;
};

#endif