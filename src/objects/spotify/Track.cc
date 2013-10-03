#include "Track.h"

#include "../../Application.h"
#include "../../SpotifyService/SpotifyService.h"

extern Application* application;

void Track::setStarred(bool starred) {
  this->starred = starred;
  auto cb = [=] () {
    //This takes an array of pointers to nodeTracks, so we need to tack the adress of the saved spotifyNodeTrack pointer.
    sp_track_set_starred(application->session, &track, 1, this->starred);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
}