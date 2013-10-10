#include "Track.h"
#include "../../Application.h"

extern Application* application;

Track::Track(sp_track* _track) : track(_track) {
  sp_track_add_ref(track);
  starred = sp_track_is_starred(application->session, track);
  name = std::string(sp_track_name(track));
  popularity = sp_track_popularity(track);
  duration = sp_track_duration(track);
  for(int i = 0; i < sp_track_num_artists(track); i++) {
    sp_artist* spArtist = sp_track_artist(track, i);
    std::shared_ptr<Artist> artist = Artist::fromCache(spArtist);
    artists.push_back(artist);
  }
  sp_album* spAlbum = sp_track_album(track);
  album = Album::fromCache(spAlbum);

  sp_link* spLink = sp_link_create_from_track(track, 0);
  char linkChar[256];
  sp_link_as_string(spLink, linkChar, 256);
  link = std::string(linkChar);
  sp_link_release(spLink);
};

void Track::setStarred(bool starred) {
  this->starred = starred;
  auto cb = [=] () {
    //This takes an array of pointers to nodeTracks, so we need to tack the adress of the saved spotifyNodeTrack pointer.
    sp_track_set_starred(application->session, &track, 1, this->starred);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
}