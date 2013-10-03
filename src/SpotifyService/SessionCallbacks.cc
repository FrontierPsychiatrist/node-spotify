#include "SessionCallbacks.h"

#include "../Application.h"
#include "../NodeCallback.h"
#include "../objects/spotify/PlaylistContainer.h"
#include "../objects/node/NodePlayer.h"
#include "../events.h"
#include "SpotifyService.h"

extern "C" {
  #include "../audio/audio.h"
}

#include <stdlib.h>
#include <string.h>
#include <iostream>

extern Application* application;

//TODO!
extern int notifyDo;

namespace spotify {
//TODO
int framesReceived = 0;
int currentSecond = 0;
}

static sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks; 

void SessionCallbacks::notifyMainThread(sp_session* session) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  pthread_mutex_lock(&application->spotifyService->notifyMutex);
  notifyDo = 1;
  pthread_cond_signal(&application->spotifyService->notifyCondition);
  pthread_mutex_unlock(&application->spotifyService->notifyMutex);
}

void SessionCallbacks::loggedIn(sp_session* session, sp_error error) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  if(SP_ERROR_OK != error) {
    std::cout << "Error logging in: " << sp_error_message(error) << std::endl;
    spotifyService->loggedOut = 1;
    return;
  } else {
    std::cout << "Logged in" << std::endl;
  }

  //The creation of the root playlist container is absolutely necessary here, otherwise following callbacks can crash.
  rootPlaylistContainerCallbacks.container_loaded = &rootPlaylistContainerLoaded;
  sp_playlistcontainer *pc = sp_session_playlistcontainer(session);
  application->playlistContainer = std::shared_ptr<PlaylistContainer>(new PlaylistContainer(pc));
  sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, application->playlistContainer.get());
}

void SessionCallbacks::loggedOut(sp_session* session) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
  spotifyService->loggedOut = 1;
}

void SessionCallbacks::rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata) {
  PlaylistContainer* playlistContainer = static_cast<PlaylistContainer*>(userdata);
  playlistContainer->loadPlaylists();
  
  //Trigger the login complete callback.
  NodeCallback* nodeCallback = new NodeCallback();
  nodeCallback->function = &application->loginCallback;
  application->asyncHandle.data  = (void*)nodeCallback;
  uv_async_send(&application->asyncHandle);
}

void SessionCallbacks::end_of_track(sp_session* session) {
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  application->nodePlayer->call(PLAYER_END_OF_TRACK);
}

static void sendTimer(int sample_rate) {
  if( spotify::currentSecond < spotify::framesReceived / sample_rate) {
    spotify::currentSecond++;
    application->nodePlayer->setCurrentSecond(spotify::currentSecond);
  }
}

int SessionCallbacks::music_delivery(sp_session *sess, const sp_audioformat *format,
                          const void *frames, int num_frames)
{
  audio_fifo_t *af = &application->audio_fifo;
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

  spotify::framesReceived += num_frames;
  sendTimer(format->sample_rate);
  return num_frames;
}