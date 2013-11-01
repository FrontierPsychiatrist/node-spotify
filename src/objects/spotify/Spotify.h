#ifndef _SPOTIFY_H
#define _SPOTIFY_H

#include <libspotify/api.h>
#include <string>
#include "SpotifyOptions.h"

class Spotify {
public:
  Spotify(SpotifyOptions options);
  ~Spotify() {};
  void login(std::string user, std::string password, bool remeberedUser, bool withRemembered);
  void logout();
  std::string rememberedUser();
private:
  sp_session* session;
  sp_session* createSession(SpotifyOptions options);
};

#endif