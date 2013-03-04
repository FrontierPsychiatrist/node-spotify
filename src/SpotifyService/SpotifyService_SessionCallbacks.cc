#include "SpotifyService_SessionCallbacks.h"

//TODO!
#include "SpotifyService_PlaylistCallbacks.h"

#include "../Callback.h"
#include "../SpotifyService.h"
#include <stdio.h>

//TODO!
extern int notifyDo;

namespace spotify {

sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks; 
sp_playlist_callbacks playlistCallbacks;

void notifyMainThread(sp_session* session) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	pthread_mutex_lock(&spotifyService->notifyMutex);
	notifyDo = 1;
	pthread_cond_signal(&spotifyService->notifyCondition);
	pthread_mutex_unlock(&spotifyService->notifyMutex);
}

void loggedIn(sp_session* session, sp_error error) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	if(SP_ERROR_OK != error) {
		fprintf(stderr, "BACKEND: Error logging in: %s\n", sp_error_message(error));
	} else {
		fprintf(stdout, "BACKEND: Service is logged in!\n");
	}

	//The creation of the root playlist container is absolutely necessary here, otherwise following callbacks can crash.
	rootPlaylistContainerCallbacks.container_loaded = &rootPlaylistContainerLoaded;
	sp_playlistcontainer *pc = sp_session_playlistcontainer(session);
	PlaylistContainer* playlistContainer = new PlaylistContainer(pc);
	spotifyService->setPlaylistContainer(playlistContainer);
	sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, playlistContainer);
}

void loggedOut(sp_session* session) {
	SpotifyService* spotifyService = static_cast<SpotifyService*>(sp_session_userdata(session));
	spotifyService->loggedOut = 1;
	fprintf(stdout, "BACKEND: Service is logged out\n");
}

void rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata) {
	PlaylistContainer* playlistContainer = static_cast<PlaylistContainer*>(userdata);
	int numPlaylists = sp_playlistcontainer_num_playlists(spPlaylistContainer);

	playlistCallbacks.playlist_state_changed = &playlistStateChanged;
	playlistCallbacks.playlist_renamed = &playlistNameChange;

	for(int i = 0; i < numPlaylists; ++i) {
		sp_playlist* spPlaylist = sp_playlistcontainer_playlist(spPlaylistContainer, i);
		Playlist* playlist = new Playlist(spPlaylist);
		sp_playlist_add_callbacks(spPlaylist, &playlistCallbacks, playlist);
		playlistContainer->addPlaylist(playlist);
	}
}

} //namespace
