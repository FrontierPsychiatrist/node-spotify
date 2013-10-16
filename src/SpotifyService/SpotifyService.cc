#include "SpotifyService.h"
#include <iostream>

#include "SessionCallbacks.h"

extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

#include "../Application.h"
extern Application* application;

sp_session_config SpotifyService::sessionConfig;
sp_session_callbacks SpotifyService::sessionCallbacks;

sp_session* SpotifyService::createSession() {
  sp_error error;
  sp_session* session;
  
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
  sessionConfig.userdata = this;

  error = sp_session_create(&sessionConfig, &session);
  if(SP_ERROR_OK != error) {
    //LOG(WARNING) << "Could not create spotify session: " << sp_error_message(error);
  }
  return session;
}

SpotifyService::SpotifyService() : loggedOut(true) {
  spotifySession = createSession();
  application->session = spotifySession;
}

void SpotifyService::login(std::string username, std::string password, bool rememberMe, bool withRemembered) {
  if(withRemembered) {
    sp_session_relogin(spotifySession);
  } else {
    sp_session_login(spotifySession, username.c_str(), password.c_str(), rememberMe, NULL);
  }
}

void SpotifyService::logout() {
  application->playlistContainer.reset();
  sp_session_logout(spotifySession);
}
