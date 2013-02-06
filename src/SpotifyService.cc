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
CallbackBase* gCallback = 0;

static void notifyMainThread(sp_session* session);
static void loggedIn(sp_session* session, sp_error error);
static void loggedOut(sp_session* session);
static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata);

/**
 * The loop running in the spotify thread.
 *
 * This functions first creates a spotify session, logs in and then loops.
 *
 * It waits for either "commands" (other classes wanting to execute libspotify API functions
 * or orders to process sp_session_process_events from notifaMainThread
 * **/
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

			while(!notifyDo && gCallback == 0) {
				if(pthread_cond_timedwait(&spotifyService->notifyCondition, &spotifyService->notifyMutex, &ts))
					break;
			}
		}
		pthread_mutex_unlock(&spotifyService->notifyMutex);
		notifyDo = 0;

		//Execute a callback from another class/thread
		if(gCallback != 0) {
			gCallback->call();
			//The callback needs to be allocated with new, so we delete it after it is used
			delete gCallback;
			gCallback = 0;
		}

		do {
			sp_session_process_events(session, &nextTimeout);
		} while(nextTimeout == 0);
		pthread_mutex_lock(&spotifyService->notifyMutex);

	}

	sp_session_release(session);
	return 0;
}


/* ################################### LIBSPOTIFY CALLBACKS ####################################### */
/**
 * Functions called by libspotify as callbacks.
 *
 * If a callback from libspotify needs to call a callback in nodeJS use uv_async_send and attach a callback to the async handle.
 * **/

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

	//The creation of the root playlist container is absolutely necessary here, otherwise following callbacks can crash.
	sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks; 
	rootPlaylistContainerCallbacks.container_loaded = &rootPlaylistContainerLoaded;
	sp_playlistcontainer *pc = sp_session_playlistcontainer(spotifyService->spotifySession);
	sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, NULL);
}

static void loggedOut(sp_session* session) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	spotifyService->loggedOut = 1;
	fprintf(stdout, "Service is logged out\n");
}

static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata) {
	int numPlaylists = sp_playlistcontainer_num_playlists(pc);
	fprintf(stdout, "Root playlist synchronized, number of Playlists: %d\n", numPlaylists);
}

/* ################## MEMBER METHODS ################################ */
SpotifyService::SpotifyService() {
	loggedOut = 0;
}

/**
 * Starts the spotify thread and logs in
 * **/
void SpotifyService::login(std::string username, std::string password) {
	pthread_create(&spotifyThread, NULL, spotifyLoop, this);
}

void SpotifyService::logout() {
	sp_session_logout(spotifySession);
}

/**
 * This executes a callback within the spotify loop. Used to call libspotify API functions from
 * other threads
 * **/
void SpotifyService::executeSpotifyAPIcall(CallbackBase* callback) {
	pthread_mutex_lock(&notifyMutex);
	gCallback = callback;
	pthread_cond_signal(&notifyCondition);
	pthread_mutex_unlock(&notifyMutex);
}
