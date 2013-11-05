#include "AlbumBrowseCallbacks.h"
#include "../objects/spotify/Album.h"
#include "../events.h"

void AlbumBrowseCallbacks::albumBrowseComplete(sp_albumbrowse* result, void* userdata) {
  Album* album = static_cast<Album*>(userdata);
  if(album->nodeObject != nullptr) {
    album->nodeObject->call(ALBUMBROWSE_COMPLETE);
  }
}
