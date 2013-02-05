#include "SpotifyService.h"
#include <stdio.h>
#include <pthread.h>
#include <mach/clock.h>
#include <mach/mach.h>

extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

/* Temporary hardcoded logindata, not to be commited */
extern const char* username;
extern const char* password;
int notifyDo = 0;

static void notifyMainThread(sp_session* session);
static void loggedIn(sp_session* session, sp_error error);
static void loggedOut(sp_session* session);
static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata);

static void* spotifyLoop(void* _spotifyService) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(_spotifyService);
	sp_error error;
	sp_session* session;

	int nextTimeout = 0;

	pthread_mutex_init(&spotifyService->notifyMutex, NULL);
	pthread_cond_init(&spotifyService->notifyCondition, NULL);

	sp_session_callbacks sessionCallbacks;
	sessionCallbacks.notify_main_thread = &notifyMainThread;
	sessionCallbacks.logged_in = &loggedIn;
	sessionCallbacks.logged_out = &loggedOut;

	sp_session_config sessionConfig;
	sessionConfig.api_version = SPOTIFY_API_VERSION;
	sessionConfig.cache_location = "tmp";
	sessionConfig.settings_location = "tmp";
	sessionConfig.application_key = spotifyAppkey;
	sessionConfig.application_key_size = spotifyAppkeySize;
	sessionConfig.user_agent = "nodejs-spotify-adapter";
	sessionConfig.callbacks = &sessionCallbacks;
	sessionConfig.userdata = spotifyService;

	error = sp_session_create(&sessionConfig, &session);
	spotifyService->spotifySession = session;
	
	if(SP_ERROR_OK != error) {
		fprintf(stderr, "Could not create Spotify session: %s\n", sp_error_message(error));
	}

	sp_session_login(session, username, password, 0, NULL);

	pthread_mutex_lock(&spotifyService->notifyMutex);
	while(!spotifyService->loggedOut) {
		if(nextTimeout == 0) {
			while(notifyDo == 0) {
				pthread_cond_wait(&spotifyService->notifyCondition, &spotifyService->notifyMutex);
			}
		} else {
			struct timespec ts;
			clock_serv_t cclock;
			mach_timespec_t mts;
			host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
			clock_get_time(cclock, &mts);
			mach_port_deallocate(mach_task_self(), cclock);
			ts.tv_sec = mts.tv_sec + nextTimeout / 1000;
			ts.tv_nsec = mts.tv_nsec + (nextTimeout % 1000) * 1000;

			while(!notifyDo) {
				if(pthread_cond_timedwait(&spotifyService->notifyCondition, &spotifyService->notifyMutex, &ts))
					break;
			}
		}
		pthread_mutex_unlock(&spotifyService->notifyMutex);

		do {
			sp_session_process_events(session, &nextTimeout);
		} while(nextTimeout == 0);
		pthread_mutex_lock(&spotifyService->notifyMutex);

	}

	sp_session_release(session);
	return 0;
}


/* ################################### CALLBACKS ####################################### */

static void notifyMainThread(sp_session* session) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	pthread_mutex_lock(&spotifyService->notifyMutex);
	notifyDo = 1;
	pthread_cond_signal(&spotifyService->notifyCondition);
	pthread_mutex_unlock(&spotifyService->notifyMutex);
}

static void loggedIn(sp_session* session, sp_error error) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	if(SP_ERROR_OK != error) {
		fprintf(stderr, "Error logging in: %s\n", sp_error_message(error));
	} else {
		fprintf(stdout, "Service is logged in!\n");
	}

	//This is absolutely necessary here, otherwise following callbacks can crash.
	sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks; 
	rootPlaylistContainerCallbacks.container_loaded = &rootPlaylistContainerLoaded;
	sp_playlistcontainer *pc = sp_session_playlistcontainer(spotifyService->spotifySession);
	sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, NULL);
}

static void loggedOut(sp_session* session) {
	fprintf(stdout, "Service is logged out\n");
}

static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata) {
	int numPlaylists = sp_playlistcontainer_num_playlists(pc);
	fprintf(stdout, "Root playlist synchronized, number of Playlists: %d\n", numPlaylists);
}

/* ################## MEMBER METHODS ################################ */
SpotifyService::SpotifyService() {

}

void SpotifyService::login(std::string username, std::string password) {
	pthread_create(&spotifyThread, NULL, spotifyLoop, this);
}
