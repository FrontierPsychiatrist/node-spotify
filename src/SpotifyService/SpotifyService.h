#ifndef SPOTIFY_SERVICE_H
#define SPOTIFY_SERVICE_H

#include <uv.h>
#include <string>
#include <libspotify/api.h>

class SpotifyService {
  template<class T> friend class SpotifyWrapped;
public:
  SpotifyService();
  ~SpotifyService();

  void login(std::string username, std::string password, bool rememberMe, bool withRemembered);
  void logout();

  bool loggedOut;
  sp_session* spotifySession;
private:
  sp_session* createSession();

  static sp_session_config sessionConfig;
  static sp_session_callbacks sessionCallbacks;
};

#endif
