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

#ifndef _SPOTIFY_SERVICE_SESSION_CALLBACKS_H
#define _SPOTIFY_SERVICE_SESSION_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>
#include <memory>
#include <v8.h>

class SessionCallbacks {
public:
  static void notifyMainThread(sp_session* session);
  static void loggedIn(sp_session* session, sp_error error);
  static void loggedOut(sp_session* session);
  static int music_delivery(sp_session *sess, const sp_audioformat *format, const void *frames, int num_frames);
  static void end_of_track(sp_session* session);
  static void handleNotify(uv_async_t* handle, int status);
  static void init();
  static void metadata_updated(sp_session* session);
  static void rootPlaylistContainerLoaded(sp_playlistcontainer* sp, void* userdata);
  static v8::Handle<v8::Function> loginCallback;
  static v8::Handle<v8::Function> logoutCallback;
  static v8::Handle<v8::Function> metadataUpdatedCallback;
private:
  static std::unique_ptr<uv_timer_t> timer;
  static std::unique_ptr<uv_async_t> notifyHandle;
  static void processEvents(uv_timer_t* timer, int status);
  static void sendTimer(int sample_rate);
};

#endif
