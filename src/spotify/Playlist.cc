#include "Playlist.h"
#include "../events.h"

void Playlist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	String::Utf8Value v8Str(value);
	playlist->name = *v8Str;
}

Handle<Value> Playlist::getName(Local<String> property, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	return String::New(playlist->name.c_str());
}

void Playlist::setId(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  //Do nothing, id cannot be changed
}

Handle<Value> Playlist::getId(Local<String> property, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	return Integer::New(playlist->id);
}

void Playlist::init(Handle<Object> target) {
	HandleScope scope;
	Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
	constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
	constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

	constructorTemplate->InstanceTemplate()->SetAccessor(String::New("name"), getName, setName);
	constructorTemplate->InstanceTemplate()->SetAccessor(String::New("id"), getId, setId);

	constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}
