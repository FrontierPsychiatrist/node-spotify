#ifndef SPOTIFY_SERVICE_H
#define SPOTIFY_SERVICE_H

#include <uv.h>
#include <string>
#include <functional>
#include <pthread.h>
#include <libspotify/api.h>

class SpotifyService {
    template<class T> friend class SpotifyWrapped;
  public:
    SpotifyService();
    ~SpotifyService();

    void login(std::string username, std::string password, bool rememberMe, bool withRemembered);
    void logout();
    void executeSpotifyAPIcall(std::function<void()> fun);

    uv_async_t callNodeThread;
    pthread_mutex_t notifyMutex;
    pthread_cond_t notifyCondition;
    int loggedOut;
    char* rememberedUser;
    
    sp_session* spotifySession;
  private:
    pthread_t spotifyThread;
};

#endif
