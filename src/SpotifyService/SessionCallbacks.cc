#include "SessionCallbacks.h"

//TODO!
//Maybe with extern instead of import?
#include "PlaylistCallbacks.h"

#include "../Callback.h"
#include "../NodeCallback.h"
#include "SpotifyService.h"
#include <stdio.h>

extern "C" {
#include "../audio/audio.h"
}

#include <stdlib.h>
#include <string.h>

//TODO!
extern int notifyDo;
extern SpotifyService* spotifyService;
PlaylistContainer* playlistContainer;
audio_fifo_t g_audiofifo;

namespace spotify {
sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks; 
sp_playlist_callbacks playlistCallbacks;

void notifyMainThread(sp_session* session) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  pthread_mutex_lock(&spotifyService->notifyMutex);
  notifyDo = 1;
  pthread_cond_signal(&spotifyService->notifyCondition);
  pthread_mutex_unlock(&spotifyService->notifyMutex);
}

void loggedIn(sp_session* session, sp_error error) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  if(SP_ERROR_OK != error) {
    fprintf(stderr, "BACKEND: Error logging in: %s\n", sp_error_message(error));
    spotifyService->loggedOut = 1;
    return;
  } else {
    fprintf(stdout, "BACKEND: Service is logged in!\n");
  }

  //The creation of the root playlist container is absolutely necessary here, otherwise following callbacks can crash.
  rootPlaylistContainerCallbacks.container_loaded = &rootPlaylistContainerLoaded;
  sp_playlistcontainer *pc = sp_session_playlistcontainer(session);
  playlistContainer = new PlaylistContainer(pc);
  sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, playlistContainer);
}

void loggedOut(sp_session* session) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  spotifyService->loggedOut = 1;
  fprintf(stdout, "BACKEND: Service is logged out\n");
}

void rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata) {
  PlaylistContainer* playlistContainer = static_cast<PlaylistContainer*>(userdata);
  playlistContainer->loadPlaylists();
  
  //Trigger the login complete callback.
  NodeCallback* nodeCallback = new NodeCallback();
  nodeCallback->function = PlaylistContainer::getContainerLoadedCallback();
  spotifyService->callNodeThread.data  = (void*)nodeCallback;
  uv_async_send(&spotifyService->callNodeThread);
}

int music_delivery(sp_session *sess, const sp_audioformat *format,
                          const void *frames, int num_frames)
{
  audio_fifo_t *af = &g_audiofifo;
  audio_fifo_data_t *afd;
  size_t s;

  if (num_frames == 0)
    return 0; // Audio discontinuity, do nothing
   
  pthread_mutex_lock(&af->mutex);

  /* Buffer one second of audio */
  if (af->qlen > format->sample_rate) {
    pthread_mutex_unlock(&af->mutex);

    return 0;
  }

  s = num_frames * sizeof(int16_t) * format->channels;

  afd = (audio_fifo_data_t*)malloc(sizeof(*afd) + s);
  memcpy(afd->samples, frames, s);

  afd->nsamples = num_frames;

  afd->rate = format->sample_rate;
  afd->channels = format->channels;

  TAILQ_INSERT_TAIL(&af->q, afd, link);
  af->qlen += num_frames;

  pthread_cond_signal(&af->cond);
  pthread_mutex_unlock(&af->mutex);

  return num_frames;
}

} //namespace
