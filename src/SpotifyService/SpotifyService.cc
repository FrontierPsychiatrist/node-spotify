#include "SpotifyService.h"

#ifdef OS_LINUX
#include <sys/time.h>
#elif OS_OSX
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "SessionCallbacks.h"

#include <glog/logging.h>

using namespace spotify;

extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

//global variables...
static std::string username;
static std::string password;

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

  sessionCallbacks.notify_main_thread = &notifyMainThread;
  sessionCallbacks.logged_in = &loggedIn;
  sessionCallbacks.logged_out = &loggedOut;
  sessionCallbacks.music_delivery = &music_delivery;
  sessionCallbacks.end_of_track = &end_of_track;

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
    LOG(WARNING) << "Could not create spotify session: " << sp_error_message(error);
  }

  spotifyService->spotifySession = session;

  sp_session_login(session, username.c_str(), password.c_str(), 0, NULL);

  pthread_mutex_lock(&spotifyService->notifyMutex);
  while(!spotifyService->loggedOut) {
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
}

/**
 * Starts the spotify thread and logs in
 * **/
void SpotifyService::login(std::string _username, std::string _password) {
  username = _username;
  password = _password;
  pthread_create(&spotifyThread, NULL, spotifyLoop, this);
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
