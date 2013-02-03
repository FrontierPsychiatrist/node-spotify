#include <stdio.h>
#include <libspotify/api.h>
#include <pthread.h>
#include <mach/clock.h>
#include <mach/mach.h>
#define COMMAND_SP_LOGOUT 1
#define COMMAND_SP_PRINTUSER 2

extern uint8_t spotifyAppkey[];
extern int spotifyAppkeySize;

/* Temporary hardcoded logindata, not to be commited */
extern const char* username;
extern const char* password;

static void* spotifyLoop(void*);

/* ################################# CALLBACKS FORWARD DECLARATION ############################### */

static void notifyMainThread(sp_session* session);
static void loggedIn(sp_session* session, sp_error error);
static void loggedOut(sp_session* session);
static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata); 

/* ############################## GLOBAL VARIABLES ########################### */

static sp_session* spotifySession;
static int notify_do = 0;
static int command = 0;
static int logged_out = 0;

static pthread_mutex_t notifyMutex;
static pthread_cond_t notifyCondition;
static pthread_t spotifyThread;

static sp_session_callbacks sessionCallbacks = {
	.notify_main_thread = &notifyMainThread,
	.logged_in = &loggedIn,
	.logged_out = &loggedOut,
};

static sp_session_config sessionConfig = {
	.api_version = SPOTIFY_API_VERSION,
	.cache_location = "tmp",
	.settings_location = "tmp",
	.application_key = spotifyAppkey,
	.application_key_size = 0,
	.user_agent = "nodejs-spotify-adapter",
	.callbacks = &sessionCallbacks,
	NULL,
};

static sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks = {
	.container_loaded = &rootPlaylistContainerLoaded,
};

void* spotifyLoop(void* nervNicht) {
	sp_error error;
	sp_session* session;

	int nextTimeout = 0;

	sessionConfig.application_key_size = spotifyAppkeySize;

	error = sp_session_create(&sessionConfig, &session);

	if(SP_ERROR_OK != error) {
		fprintf(stderr, "Could not create Spotify session: %s\n", sp_error_message(error));
	}
	
	spotifySession = session;

	pthread_mutex_init(&notifyMutex, NULL);
	pthread_cond_init(&notifyCondition, NULL);

	sp_session_login(session, username, password, 0, NULL);
	pthread_mutex_lock(&notifyMutex);
	int log_out_called = 0;
	while(!logged_out) {
		if(nextTimeout == 0) {
			while(notify_do == 0) {
				pthread_cond_wait(&notifyCondition, &notifyMutex);
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

			while(!notify_do && !command) {
				if(pthread_cond_timedwait(&notifyCondition, &notifyMutex, &ts))
					break;
			}
		}

		if(command) {
			if(command == COMMAND_SP_LOGOUT) {
				sp_session_logout(spotifySession);
				log_out_called = 1;
			} else if(command == COMMAND_SP_PRINTUSER) {
				sp_user* user = sp_session_user(spotifySession);
				const char* name = sp_user_display_name(user);
				fprintf(stdout, "User: %s\n", name);
			}
		}
		command = 0;
		notify_do = 0;
		pthread_mutex_unlock(&notifyMutex);

		do {
			sp_session_process_events(session, &nextTimeout);
		} while(nextTimeout == 0);

		pthread_mutex_lock(&notifyMutex);
	}
	sp_session_release(session);
	return 0; 
}

/* ################################### CALLBACKS ####################################### */

static void notifyMainThread(sp_session* session) {
	pthread_mutex_lock(&notifyMutex);
	notify_do = 1;
	pthread_cond_signal(&notifyCondition);
	pthread_mutex_unlock(&notifyMutex);
}

static void loggedIn(sp_session* session, sp_error error) {
	if(SP_ERROR_OK != error) {
		fprintf(stderr, "Error logging in: %s\n", sp_error_message(error));
	} else {
		fprintf(stdout, "Service is logged in!\n");
	}

	//This is absolutely necessary here, otherwise following callbacks can crash.
	sp_playlistcontainer *pc = sp_session_playlistcontainer(spotifySession);
	sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, NULL);
}

static void loggedOut(sp_session* session) {
	fprintf(stdout, "Service is logged out\n");
	logged_out = 0;
}

static void rootPlaylistContainerLoaded(sp_playlistcontainer* pc, void* userdata) {
	int numPlaylists = sp_playlistcontainer_num_playlists(pc);
	fprintf(stdout, "Root playlist synchronized, number of Playlists: %d\n", numPlaylists);
}

/* ################################# EXPORTS ########################### */

void printUser() {
	pthread_mutex_lock(&notifyMutex);
	command = COMMAND_SP_PRINTUSER;
	pthread_cond_signal(&notifyCondition);
	pthread_mutex_unlock(&notifyMutex);
}

void logout() {
	pthread_mutex_lock(&notifyMutex);
	command = COMMAND_SP_LOGOUT;
	pthread_cond_signal(&notifyCondition);
	pthread_mutex_unlock(&notifyMutex);
}

void login() {
	pthread_create(&spotifyThread, NULL, spotifyLoop, NULL);
}

void printPlaylistnames() {
	sp_playlistcontainer *pc = sp_session_playlistcontainer(spotifySession);
	sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, NULL);
	int numPlaylists = sp_playlistcontainer_num_playlists(pc);
	fprintf(stdout, "Number of Playlists: %d\n", numPlaylists);
	for(int i = 0; i < numPlaylists; ++i) {
		sp_playlist* playlist  = sp_playlistcontainer_playlist(pc, i);
		fprintf(stdout, "Playlist: %s\n", sp_playlist_name(playlist));
	}
}
