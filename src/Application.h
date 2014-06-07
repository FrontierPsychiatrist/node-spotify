#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <libspotify/api.h>
#include <memory>
#include "objects/spotify/PlaylistContainer.h"

struct Application {
  sp_session* session;
  std::shared_ptr<PlaylistContainer> playlistContainer;
};

#endif
