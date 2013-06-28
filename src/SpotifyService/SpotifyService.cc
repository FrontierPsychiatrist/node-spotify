#include "SpotifyService.h"

#ifdef OS_LINUX
#include <sys/time.h>
#elif OS_OSX
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "SessionCallbacks.h"
#include <iostream>

extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

int notifyDo = 0;
std::function<void()> gFun;

static sp_session_config sessionConfig;
static sp_session_callbacks sessionCallbacks;

/**
 * The loop running in the spotify thread.
 *
 * This functions first creates a spotify session, logs in and then loops.
 *
 * It waits for either "commands" (other classes wanting to execute libspotify API functions
 * or orders to process sp_session_process_events from notifaMainThread
 * **/
static void* spotifyLoop(void* _spotifyService) {
  SpotifyService* spotifyService = static_cast<SpotifyService*>(_spotifyService);
  sp_error error;
  sp_session* session;

  int nextTimeout = 0;
  spotifyService->loggedOut = 0;
  pthread_mutex_init(&spotifyService->notifyMutex, NULL);
  pthread_cond_init(&spotifyService->notifyCondition, NULL);

  sessionCallbacks.notify_main_thread = &SessionCallbacks::notifyMainThread;
  sessionCallbacks.logged_in = &SessionCallbacks::loggedIn;
  sessionCallbacks.logged_out = &SessionCallbacks::loggedOut;
  sessionCallbacks.music_delivery = &SessionCallbacks::music_delivery;
  sessionCallbacks.end_of_track = &SessionCallbacks::end_of_track;

  sessionConfig.api_version = SPOTIFY_API_VERSION;
  sessionConfig.cache_location = "tmp";
  sessionConfig.settings_location = "tmp";
  sessionConfig.application_key = spotifyAppkey;
  sessionConfig.application_key_size = spotifyAppkeySize;
  sessionConfig.user_agent = "nodejs-spotify-adapter";
  sessionConfig.callbacks = &sessionCallbacks;
  //sessionConfig.tracefile = "spotify-tracefile";
  sessionConfig.userdata = spotifyService;

  error = sp_session_create(&sessionConfig, &session);
  
  if(SP_ERROR_OK != error) {
    //LOG(WARNING) << "Could not create spotify session: " << sp_error_message(error);
  }

  spotifyService->spotifySession = session;

  size_t maxUsernameLength = 255;
  char rememberedUser[maxUsernameLength];
  int usernameLength = sp_session_remembered_user(session, rememberedUser, maxUsernameLength);
  if(usernameLength != -1) {
    spotifyService->rememberedUser = rememberedUser;
  }

  bool beforeLogin = true;

  pthread_mutex_lock(&spotifyService->notifyMutex);
  while(beforeLogin || !spotifyService->loggedOut) {
    if(nextTimeout == 0) {
      while(notifyDo == 0) {
        pthread_cond_wait(&spotifyService->notifyCondition, &spotifyService->notifyMutex);
      }
    } else {
      struct timespec ts;
#ifdef OS_LINUX      
      clock_gettime(CLOCK_REALTIME, &ts);
      ts.tv_sec = ts.tv_sec + nextTimeout / 1000;
      ts.tv_nsec = ts.tv_nsec + (nextTimeout % 1000) * 1000;
#elif OS_OSX
      clock_serv_t cclock;
      mach_timespec_t mts;
      host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
      clock_get_time(cclock, &mts);
      mach_port_deallocate(mach_task_self(), cclock);
      ts.tv_sec = mts.tv_sec + nextTimeout / 1000;
      ts.tv_nsec = mts.tv_nsec + (nextTimeout % 1000) * 1000;
#endif

      while(!notifyDo && !gFun) {
        if(pthread_cond_timedwait(&spotifyService->notifyCondition, &spotifyService->notifyMutex, &ts))
          break;
      }
    }
    pthread_mutex_unlock(&spotifyService->notifyMutex);
    notifyDo = 0;

    //Execute a callback from another class/thread
    if(gFun) {
      gFun();
      gFun = nullptr;
    }

    do {
      sp_session_process_events(session, &nextTimeout);
    } while(nextTimeout == 0);
    pthread_mutex_lock(&spotifyService->notifyMutex);

  }

  sp_session_release(session);
  pthread_mutex_destroy(&spotifyService->notifyMutex);
  pthread_cond_destroy(&spotifyService->notifyCondition);
  //TODO: this causes a crash when logging in a second time. why?
  //uv_close((uv_handle_t*)&spotifyService->callNodeThread, NULL);
  return 0;
}

SpotifyService::SpotifyService() {
  loggedOut = 0;
  rememberedUser = 0;
  pthread_create(&spotifyThread, NULL, spotifyLoop, this);
}

/**
 * Starts the spotify thread and logs in
 * **/
void SpotifyService::login(std::string username, std::string password, bool rememberMe, bool withRemembered) {
  auto cb = [=] () {
    if(withRemembered) {
      sp_session_relogin(spotifySession);
    } else {
      sp_session_login(spotifySession, username.c_str(), password.c_str(), rememberMe, NULL);
    }
  };
  executeSpotifyAPIcall(cb);
}

void SpotifyService::logout() {
  sp_session_logout(spotifySession);
}

/**
 * This executes a callback within the spotify loop. Used to call libspotify API functions from
 * other threads
 * **/
void SpotifyService::executeSpotifyAPIcall(std::function<void()> fun) {
  pthread_mutex_lock(&notifyMutex);
  gFun = fun;
  pthread_cond_signal(&notifyCondition);
  pthread_mutex_unlock(&notifyMutex);
}
