#ifndef _PLAYER_H
#define _PLAYER_H

#include <node.h>

#include "SpotifyWrapped.h"
#include "../Callback.h"
#include "Playlist.h"

using namespace v8;

class Player : public SpotifyWrapped<Player> {
	private:
		Playlist* playlist;
		int currentTrack;
		bool isPaused;
		
		void spotifyPause();
		void spotifyStop();
		void spotifyResume();
		void spotifyPlay();
		
  public:
    Player(uv_async_t* _handle) : SpotifyWrapped(_handle) {};
		//NodeJS visible methods
		static Handle<Value> stop(const Arguments& args);
		static Handle<Value> pause(const Arguments& args);
		static Handle<Value> resume(const Arguments& args);
		static Handle<Value> play(const Arguments& args);
		static Handle<Value> nextTrack(const Arguments& args);

		void nextTrack();
		/**
		*	Callback track finished playing:
		*		- remove track from queue
		*		- play next track
		*		- update currentTrackPosition if necessary
		**/
		
		
		/**
		*	Communication with frontend:
		*		-
		**/
		static void init(Handle<Object> target);
};

#endif
