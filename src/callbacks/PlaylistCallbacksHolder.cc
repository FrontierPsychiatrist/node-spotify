#include "PlaylistCallbacksHolder.h"
#include "../objects/node/NodeTrackExtended.h"
#include "../objects/node/NodeUser.h"
#include "../objects/spotify/TrackExtended.h"
#include "../objects/spotify/Playlist.h"
#include "../objects/spotify/User.h"

#include <memory>

PlaylistCallbacksHolder::PlaylistCallbacksHolder(Nan::ObjectWrap* _userdata, sp_playlist* _playlist) : userdata(_userdata), playlist(_playlist) {
  playlistCallbacks = new sp_playlist_callbacks();
}

PlaylistCallbacksHolder::~PlaylistCallbacksHolder() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  delete playlistCallbacks;
}

void PlaylistCallbacksHolder::playlistRenamed(sp_playlist* spPlaylist, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle() };
  holder->playlistRenamedCallback.Call(2, argv);
}

void PlaylistCallbacksHolder::tracksAdded(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Array> nodeTracks = Nan::New<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    NodeTrack* nodeTrackExtended = new NodeTrackExtended(std::make_shared<TrackExtended>(tracks[i], spPlaylist, position + i));
    nodeTracks->Set(Nan::New<Number>(i), nodeTrackExtended->createInstance());
  }
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), nodeTracks, Nan::New<Number>(position) };
  holder->tracksAddedCallback.Call(4, argv);
}

void PlaylistCallbacksHolder::tracksMoved(sp_playlist* spPlaylist, const int* tracks, int num_tracks, int new_position, void *userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Array> movedTrackIndices = Nan::New<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    movedTrackIndices->Set(Nan::New<Number>(i), Nan::New<Number>(tracks[i]));
  }
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), movedTrackIndices, Nan::New<Number>(new_position) };
  holder->tracksMovedCallback.Call(4, argv);
}

void PlaylistCallbacksHolder::tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Array> removedTrackIndexes = Nan::New<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    removedTrackIndexes->Set(Nan::New<Number>(i), Nan::New<Number>(tracks[i]));
  }
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), removedTrackIndexes };
  holder->tracksRemovedCallback.Call(3, argv);
}

void PlaylistCallbacksHolder::trackCreatedChanged(sp_playlist* spPlaylist, int position, sp_user* spUser, int when, void* userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  double date = (double)when * 1000;
  NodeUser* nodeUser = new NodeUser(std::unique_ptr<User>(new User(spUser)));
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), Nan::New<Integer>(position), nodeUser->handle(), Nan::New<Date>(date).ToLocalChecked() };
  holder->trackCreatedChangedCallback.Call(5, argv);
}

void PlaylistCallbacksHolder::trackSeenChanged(sp_playlist* spPlaylist, int position, bool seen, void* userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), Nan::New<Integer>(position), Nan::New<Boolean>(seen) };
  holder->trackSeenChangedCallback.Call(4, argv);
}

void PlaylistCallbacksHolder::trackMessageChanged(sp_playlist* spPlaylist, int position, const char* message, void* userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Local<Value> argv[] = { Nan::Undefined(), holder->userdata->handle(), Nan::New<Integer>(position), Nan::New<String>(message).ToLocalChecked() };
  holder->trackMessageChangedCallback.Call(4, argv);
}

void PlaylistCallbacksHolder::setCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  
  if(!playlistRenamedCallback.IsEmpty()) {
    playlistCallbacks->playlist_renamed = &PlaylistCallbacksHolder::playlistRenamed;  
  }
  if(!tracksAddedCallback.IsEmpty()) {
    playlistCallbacks->tracks_added = &PlaylistCallbacksHolder::tracksAdded;  
  }
  if(!tracksMovedCallback.IsEmpty()) {
    playlistCallbacks->tracks_moved = &PlaylistCallbacksHolder::tracksMoved;  
  }
  if(!tracksRemovedCallback.IsEmpty()) {
    playlistCallbacks->tracks_removed = &PlaylistCallbacksHolder::tracksRemoved;  
  }
  if(!trackCreatedChangedCallback.IsEmpty()) {
    playlistCallbacks->track_created_changed = &PlaylistCallbacksHolder::trackCreatedChanged;
  }
  if(!trackSeenChangedCallback.IsEmpty()) {
    playlistCallbacks->track_seen_changed = &PlaylistCallbacksHolder::trackSeenChanged;
  }
  if(!trackMessageChangedCallback.IsEmpty()) {
    playlistCallbacks->track_message_changed = &PlaylistCallbacksHolder::trackMessageChanged;
  }
  sp_playlist_add_callbacks(playlist, playlistCallbacks, this);
}

void PlaylistCallbacksHolder::unsetCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
}
