#ifndef _PLAYER_H
#define _PLAYER_H

#include <libspotify/api.h>

#include "NodeWrapped.h"
#include "Track.h"

using namespace v8;

class Player : public NodeWrapped<Player> {
	private:
		Track* track;
		int currentSecond;
		bool isPaused;
  public:
    Player() : NodeWrapped() {};
		//NodeJS visible methods
		static Handle<Value> stop(const Arguments& args);
		static Handle<Value> pause(const Arguments& args);
		static Handle<Value> resume(const Arguments& args);
		static Handle<Value> play(const Arguments& args);
		static Handle<Value> seek(const Arguments& args);
		static Handle<Value> getCurrentSecond(Local<String> property, const AccessorInfo& info);

		void setCurrentSecond(int _currentSecond);
			
		static void init(Handle<Object> target);
};

#endif
