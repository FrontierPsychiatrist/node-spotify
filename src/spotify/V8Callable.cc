#include "V8Callable.h"
#include "../NodeCallback.h"

void V8Callable::on(std::string name, v8::Persistent<v8::Function> callback) {
	callbacks.insert( std::pair<std::string, v8::Persistent<v8::Function> >( name, callback) );
}

void V8Callable::call(std::string name, v8::Handle<v8::Object>* object) {
	std::map< std::string, v8::Persistent<v8::Function> >::iterator it;
	it = callbacks.find(name);

	//Check if a callback for the given name was found
	if(it != callbacks.end()) {
		//Get the adress of the callback function and send it to the node thread
		//This needs to be the adress from the map element, otherwise we would pass the adress of a local and it fails on the node side.
		v8::Persistent<v8::Function>* fun = &it->second;

    NodeCallback* nodeCallback = new NodeCallback();
    nodeCallback->object = object;
    nodeCallback->function = fun;
		asyncHandle->data  = (void*)nodeCallback;
		uv_async_send(asyncHandle);
	}
}
