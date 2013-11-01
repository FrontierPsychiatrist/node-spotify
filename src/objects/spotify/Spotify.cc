#include "Spotify.h"
#include "../../Application.h"
#include "../../Callbacks/SessionCallbacks.h"

extern Application* application;
static sp_session_config sessionConfig;
static sp_session_callbacks sessionCallbacks;
extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

Spotify::Spotify(SpotifyOptions options) {
  session = createSession(options);
  application->session = session;
};

sp_session* Spotify::createSession(SpotifyOptions options) {
  sp_error error;
  sp_session* session;

  sessionCallbacks.notify_main_thread = &SessionCallbacks::notifyMainThread;
  sessionCallbacks.logged_in = &SessionCallbacks::loggedIn;
  sessionCallbacks.logged_out = &SessionCallbacks::loggedOut;
  sessionCallbacks.music_delivery = &SessionCallbacks::music_delivery;
  sessionCallbacks.end_of_track = &SessionCallbacks::end_of_track;

  sessionConfig.api_version = SPOTIFY_API_VERSION;
  sessionConfig.cache_location = options.cacheFolder.c_str();
  sessionConfig.settings_location = options.settingsFolder.c_str();
  sessionConfig.application_key = spotifyAppkey;
  sessionConfig.application_key_size = spotifyAppkeySize;
  sessionConfig.user_agent = "nodejs-spotify-adapter";
  sessionConfig.callbacks = &sessionCallbacks;
  if(!options.traceFile.empty()) {
    sessionConfig.tracefile = options.traceFile.c_str();
  }
  sessionConfig.userdata = this;

  error = sp_session_create(&sessionConfig, &session);
  if(SP_ERROR_OK != error) {
    //LOG(WARNING) << "Could not create spotify session: " << sp_error_message(error);
  }
  return session;
}

void Spotify::login(std::string username, std::string password, bool rememberMe, bool withRemembered) {
  if(withRemembered) {
    sp_session_relogin(session);
  } else {
    sp_session_login(session, username.c_str(), password.c_str(), rememberMe, NULL);
  }
}

void Spotify::logout() {
  application->playlistContainer.reset();
  sp_session_logout(session);
}

std::string Spotify::rememberedUser() {
  std::string rememberedUserString;
  int maxUsernameLength = 256;
  char rememberedUser[maxUsernameLength];
  int usernameLength = sp_session_remembered_user(session, rememberedUser, maxUsernameLength);
  if(usernameLength != -1) {
    rememberedUserString = std::string(rememberedUser);
  }
  return rememberedUserString;
}