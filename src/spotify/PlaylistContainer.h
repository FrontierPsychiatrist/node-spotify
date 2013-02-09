#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H

#include <libspotify/api.h>
#include <vector>
#include "Playlist.h"


class PlaylistContainer {
	public:
		PlaylistContainer(sp_playlistcontainer* _playlistContainer) : playlistContainer(_playlistContainer) {};
		void addPlaylist(Playlist* const playlist) { playlists.push_back(playlist); };
		std::vector<Playlist*>& getPlaylists() { return playlists; };
	private:
		sp_playlistcontainer* playlistContainer;
		std::vector<Playlist*> playlists;
};
#endif
