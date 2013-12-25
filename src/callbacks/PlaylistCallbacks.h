/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>

class PlaylistCallbacks {
public:
  static void playlistStateChanged(sp_playlist* _playlist, void* userdata);
  static void playlistNameChange(sp_playlist* spPlaylist, void* userdata);
  static void tracksAdded(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata);
  static void tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata);
  static void tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata);
  /* staticvoid playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata);
  static void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata);*/
};

#endif
