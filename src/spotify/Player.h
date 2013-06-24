#ifndef _PLAYER_H
#define _PLAYER_H

#include <libspotify/api.h>

#include "NodeWrapped.h"
#include "../Callback.h"
#include "Playlist.h"
#include "Track.h"

using namespace v8;

class Player : public NodeWrapped<Player> {
	private:
		Playlist* currentPlaylist;
		int currentTrackPosition;
		Track* currentTrack;
		bool isPaused;
		char* currentAlbumCoverBase64;
		int currentSecond;
		
		void spotifyPause();
		void spotifyStop();
		void spotifyResume();
		void spotifyPlay();
		
		void changeAndPlayTrack();
  public:
    Player() : NodeWrapped(), currentAlbumCoverBase64(0) {};
		//NodeJS visible methods
		static Handle<Value> stop(const Arguments& args);
		static Handle<Value> pause(const Arguments& args);
		static Handle<Value> resume(const Arguments& args);
		static Handle<Value> play(const Arguments& args);
		static Handle<Value> nextTrack(const Arguments& args);
		static Handle<Value> staticOn(const Arguments& args);
		static Handle<Value> getCurrentSecond(Local<String> property, const AccessorInfo& info);
		static Handle<Value> getCurrentlyPlayingData(const Arguments& args);

		void nextTrack();
		void processImage(sp_image* image);
		void setCurrentSecond(int _currentSecond);
		/**
		*	Callback track finished playing:
		*		- remove track from queue
		*		- play next track
		*		- update currentTrackPosition if necessary
		**/
		
		static void init(Handle<Object> target);
};

#endif
