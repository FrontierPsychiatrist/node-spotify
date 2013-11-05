#include "ArtistBrowseCallbacks.h"
#include "../events.h"

#include "../objects/spotify/Artist.h"

void ArtistBrowseCallbacks::artistBrowseComplete(sp_artistbrowse* result, void* userdata) {
  Artist* artist = static_cast<Artist*>(userdata);
  if(artist->nodeObject != nullptr) {
    artist->nodeObject->call(ARTISTBROWSE_COMPLETE);
  }
}