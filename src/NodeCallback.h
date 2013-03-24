#ifndef _NODECALLBACK_H
#define _NODECALLBACK_H

#include <v8.h>
#include "spotify/SpotifyWrapped.h"

struct NodeCallback {
  SpotifyWrapped* object;
  v8::Persistent<v8::Function>* function;
};

#endif
