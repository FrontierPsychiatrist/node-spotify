#ifndef _SPOTIFY_WRAPPED_H
#define _SPOTIFY_WRAPPED_H

#include "NodeWrapped.h"

#include <assert.h>
#include <pthread.h>

/**
 * A base class for libspotify objects wrapped in JavaScript objects.
 * SpotifyWrapped objects must only be created in the spotify thread
 **/
template <class T>
class SpotifyWrapped : public NodeWrapped<T> {
  public:
    SpotifyWrapped() : NodeWrapped<T>() {

    }
};

#endif