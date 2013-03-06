#include "PlaylistContainer.h"

Persistent<Function> PlaylistContainer::containerLoadedCallback;

void PlaylistContainer::containerLoaded() {
	HandleScope scope;
	const unsigned int argc = 0;
	Local<Value> argv[argc] = { };
	if(!PlaylistContainer::containerLoadedCallback.IsEmpty() && PlaylistContainer::containerLoadedCallback->IsCallable()) {
		PlaylistContainer::containerLoadedCallback->Call(Context::GetCurrent()->Global(), argc, argv);
	}
	scope.Close(Undefined());
}
