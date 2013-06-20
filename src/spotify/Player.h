#ifndef _PLAYER_H
#define _PLAYER_H

#include <libspotify/api.h>

#include "SpotifyWrapped.h"
#include "../Callback.h"
#include "Playlist.h"

using namespace v8;

class Player : public SpotifyWrapped<Player> {
	private:
		Playlist* currentPlaylist;
		int currentTrackPosition;
		Track* currentTrack;
		bool isPaused;
		char* currentAlbumCoverBase64;
		
		void spotifyPause();
		void spotifyStop();
		void spotifyResume();
		void spotifyPlay();
		
		void changeAndPlayTrack();
  public:
    Player() : SpotifyWrapped() {};
		//NodeJS visible methods
		static Handle<Value> stop(const Arguments& args);
		static Handle<Value> pause(const Arguments& args);
		static Handle<Value> resume(const Arguments& args);
		static Handle<Value> play(const Arguments& args);
		static Handle<Value> nextTrack(const Arguments& args);
		static Handle<Value> getCurrentTrack(const Arguments& args);

		void nextTrack();
		void processImage(sp_image* image);
		/**
		*	Callback track finished playing:
		*		- remove track from queue
		*		- play next track
		*		- update currentTrackPosition if necessary
		**/
		
		static void init(Handle<Object> target);
};

#endif
