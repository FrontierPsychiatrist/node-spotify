#include "V8Callable.h"

void V8Callable::on(std::string name, v8::Persistent<v8::Function> callback) {
	callbacks.insert( std::pair<std::string, v8::Persistent<v8::Function> >( name, callback) );
}

void V8Callable::call(std::string name) {
	std::map< std::string, v8::Persistent<v8::Function> >::iterator it;
	it = callbacks.find(name);

	//Check if a callback for the given name was found
	if(it != callbacks.end()) {
		//Get the adress of the callback function and send it to the node thread
		//TODO: SpotifyWrapped objects should be send also so the scope for the function call can be set correct. So a Callback struct is needed and a new parameter for this method
		v8::Persistent<v8::Function>* fun = &it->second;
		asyncHandle->data  = (void*)fun;
		uv_async_send(asyncHandle);
	}	
}
