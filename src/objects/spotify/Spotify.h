#ifndef _SPOTIFY_H
#define _SPOTIFY_H

#include "SpotifyOptions.h"
#include "User.h"
#include "../../audio/AudioHandler.h"

#include <libspotify/api.h>
#include <string>
#include <memory>

class Spotify {
friend class AudioHandler;
friend class NodeSpotify;
friend class NodeAudioHandler;
#ifndef NODE_SPOTIFY_NATIVE_SOUND
friend class Player;
#endif

public:
  Spotify(SpotifyOptions options);
  ~Spotify() {}
  void login(std::string user, std::string password, bool remeberedUser, bool withRemembered);
  void logout();
  std::string rememberedUser();
  std::shared_ptr<User> sessionUser();
private:
  std::unique_ptr<AudioHandler> audioHandler;
  sp_session* session;
  sp_session* createSession(SpotifyOptions options);
};

#endif
