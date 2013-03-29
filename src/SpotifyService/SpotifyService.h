#ifndef SPOTIFY_SERVICE_H
#define SPOTIFY_SERVICE_H

#include <uv.h>
#include <string>
#include <libspotify/api.h>
#include "../Callback.h"
#include "../spotify/PlaylistContainer.h"

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

    void setPlaylistContainer(PlaylistContainer* playlistContainer);
    PlaylistContainer* getPlaylistContainer();

    void setSpotifySession(sp_session* spotifySession);
  private:
    PlaylistContainer* playlistContainer;
    sp_session* spotifySession;
    pthread_t spotifyThread;
};

#endif
