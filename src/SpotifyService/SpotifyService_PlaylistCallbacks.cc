#include "SpotifyService_PlaylistCallbacks.h"
#include "../spotify/Playlist.h"
#include "../Callback.h"
#include "../SpotifyService.h"

extern SpotifyService* spotifyService;

namespace spotify {

void playlistNameChange(sp_playlist* spPlaylist, void* userdata) {
	Playlist* playlist = static_cast<Playlist*>(userdata);
	playlist->name = std::string(sp_playlist_name(spPlaylist));
	Callback<Playlist>* nodeCallback = new Callback<Playlist>(playlist, &Playlist::nameChange);
	spotifyService->callNodeThread.data  = (void*)nodeCallback;
	uv_async_send(&spotifyService->callNodeThread);
}

void playlistStateChanged(sp_playlist* _playlist, void* userdata) {
	Playlist* playlist = static_cast<Playlist*>(userdata);
	if(sp_playlist_is_loaded(_playlist)) {
		playlist->name = std::string(sp_playlist_name(_playlist));
	}
}

}
