#ifndef SPOTIFY_SERVICE_H
#define SPOTIFY_SERVICE_H

#include <uv.h>
#include <string>
#include <libspotify/api.h>
#include "../Callback.h"

class SpotifyService {
  public:
    SpotifyService();
    ~SpotifyService();

    void login(std::string username, std::string password);
    void logout();
    void executeSpotifyAPIcall(CallbackBase* callback);

    uv_async_t callNodeThread;
    pthread_mutex_t notifyMutex;
    pthread_cond_t notifyCondition;
    int loggedOut;

    void setSpotifySession(sp_session* spotifySession);
    sp_session* getSpotifySession() { return spotifySession; };
  private:
    sp_session* spotifySession;
    pthread_t spotifyThread;
};

#endif
