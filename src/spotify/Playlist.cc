#include "Playlist.h"

Persistent<Function> Playlist::constructor;

Playlist::~Playlist() {
	fprintf(stdout, "%s to be destroyed\n", name.c_str());
}

void Playlist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	String::Utf8Value v8Str(value);
	playlist->name = *v8Str;
}
Handle<Value> Playlist::getName(Local<String> property, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	return String::New(playlist->name.c_str());
}

Handle<Value> Playlist::getHandle() {
	HandleScope scope;
	//check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
	if(handle_.IsEmpty()) {
		Local<Object> o = constructor->NewInstance();
		this->Wrap(o);
	}
	return scope.Close(handle_);
}

void Playlist::init(Handle<Object> target) {
	HandleScope scope;
	Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
	constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
	constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

	constructorTemplate->InstanceTemplate()->SetAccessor(String::New("name"), getName, setName);
	constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}
