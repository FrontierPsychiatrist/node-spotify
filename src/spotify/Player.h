#ifndef _PLAYER_H
#define _PLAYER_H

#include <node.h>
#include <vector>

#include "SpotifyWrapped.h"
#include "../Callback.h"
#include "Track.h"

using namespace v8;

class Player : public SpotifyWrapped<Player> {
	private:
		std::vector<Track*> playQueue;
		int currentTrackPosition;
		bool isPaused;
		
		void spotifyPause();
		void spotifyStop();
		void spotifyResume();
		void spotifyPlay();
		/**
		* playlistID: The playlist from which to add tracks
		* trackID: the track in the playlist from which on to add tracks
		**/
		void internAddTracks(int playlistId, int trackId);
  public:
    Player(uv_async_t* _handle) : SpotifyWrapped(_handle) {};
		//NodeJS visible methods
		static Handle<Value> stop(const Arguments& args);
		static Handle<Value> pause(const Arguments& args);
		static Handle<Value> resume(const Arguments& args);
		static Handle<Value> play(const Arguments& args);
		
		static Handle<Value> addTracks(const Arguments& args);
		
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
