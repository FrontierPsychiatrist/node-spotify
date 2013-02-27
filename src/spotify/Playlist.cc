#include "Playlist.h"

Persistent<Function> Playlist::constructor;

void Playlist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	String::Utf8Value v8Str(value);
	playlist->name = *v8Str;
}
Handle<Value> Playlist::getName(Local<String> property, const AccessorInfo& info) {
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
	return String::New(playlist->name.c_str());
}

Handle<Object> Playlist::getHandle() {
	HandleScope scope;
	//check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
	if(handle_.IsEmpty()) {
		Local<Object> o = constructor->NewInstance();
		this->Wrap(o);
	}
	return scope.Close(handle_);
}

Handle<Value> Playlist::onNameChange(const Arguments& args) {
	HandleScope scope;
	Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(args.This());
	Handle<Function> fun = Handle<Function>::Cast(args[0]);
	playlist->nameChangeCallback = Persistent<Function>::New(fun);
	return scope.Close(Undefined());
}

void Playlist::init(Handle<Object> target) {
	HandleScope scope;
	Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
	constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
	constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

	constructorTemplate->InstanceTemplate()->SetAccessor(String::New("name"), getName, setName);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "onNameChange", onNameChange);
	constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}

void Playlist::nameChange() {
	HandleScope scope;
	const unsigned int argc = 0;
	Local<Value> argv[argc] = { };
	if(!nameChangeCallback.IsEmpty() && nameChangeCallback->IsCallable()) {
		nameChangeCallback->Call(getHandle(), argc, argv);
	}
	scope.Close(Undefined());
}
