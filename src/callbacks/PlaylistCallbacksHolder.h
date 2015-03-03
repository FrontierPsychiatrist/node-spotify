#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_HOLDER_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_HOLDER_H

#include <node.h>

#include <libspotify/api.h>
#include <nan.h>
#include <initializer_list>
#include <memory>

using namespace v8;

class PlaylistCallbacksHolder {
private:
  node::ObjectWrap* userdata;
  sp_playlist* playlist;
  sp_playlist_callbacks* playlistCallbacks;
  void call(std::unique_ptr<NanCallback>& callback, std::initializer_list<Handle<Value>> args);
public:
  PlaylistCallbacksHolder(node::ObjectWrap* userdata, sp_playlist* playlist);
  ~PlaylistCallbacksHolder();

  //libspotify callback functions.
  static void playlistRenamed(sp_playlist* spPlaylist, void* userdata);
  static void tracksAdded(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata);
  static void tracksMoved(sp_playlist* playlist, const int* tracks, int num_tracks, int new_position, void *userdata);
  static void tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata);
  static void trackCreatedChanged(sp_playlist* spPlaylist, int position, sp_user* spUser, int when, void* userdata);
  static void trackSeenChanged(sp_playlist* spPlaylist, int position, bool seen, void* userdata);
  static void trackMessageChanged(sp_playlist* spPlaylist, int position, const char* message, void* userdata);
  
  std::unique_ptr<NanCallback> playlistRenamedCallback;
  std::unique_ptr<NanCallback> tracksAddedCallback;
  std::unique_ptr<NanCallback> tracksMovedCallback;
  std::unique_ptr<NanCallback> tracksRemovedCallback;
  std::unique_ptr<NanCallback> trackCreatedChangedCallback;
  std::unique_ptr<NanCallback> trackSeenChangedCallback;
  std::unique_ptr<NanCallback> trackMessageChangedCallback;
  /**
    Register the callbacks with libspotify. Will first remove old registered callbacks.
  **/
  void setCallbacks();
  /**
    Unregister all callbacks.
  **/
  void unsetCallbacks();
};

#endif
