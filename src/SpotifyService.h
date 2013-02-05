#ifndef SPOTIFY_SERVICE_H
#define SPOTIFY_SERVICE_H

#include <uv.h>
#include <v8.h>
#include <string>
#include <libspotify/api.h>
#include "Callback.h"

class SpotifyService {
	public:
		SpotifyService();
		~SpotifyService();
		void login(std::string username, std::string password);
		void logout();
		void executeSpotifyAPIcall(CallbackBase* callback);

		uv_async_t nodeCallbackNotifier;
		pthread_mutex_t notifyMutex;
		pthread_cond_t notifyCondition;
		pthread_t spotifyThread;
		int loggedOut;
		sp_session* spotifySession;
	private:
		v8::Handle<v8::Value> nodeLoggedInCallback();
};

#endif
