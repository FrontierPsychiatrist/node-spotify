#ifndef _SPOTIFY_WRAPPED_H
#define _SPOTIFY_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>

#include "../NodeCallback.h"

#include <iostream>

/**
 * A class used as a base class for wrapping libspotify types to V8 objects.
**/
template <class T>
class SpotifyWrapped : public node::ObjectWrap {
  template <class S> friend class StaticCallbackSetter;

  public:
    SpotifyWrapped(uv_async_t* _handle) : asyncHandle(_handle) {
      pthread_mutex_init(&mutex, NULL);
      pthread_cond_init(&condition, NULL);
    }; 

    ~SpotifyWrapped() {
      std::cout << "Destructor called! Please check!" << std::endl;
    };

    v8::Handle<v8::Object> getV8Object() {
      //We cannot open a new HandleScope here, as this gets called in the spotify thread!
      //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
      if(handle_.IsEmpty()) {
        v8::Persistent<v8::Object> o = v8::Persistent<v8::Object>::New(constructor->NewInstance());
        this->Wrap(o);
      }
      return handle_;
    }

    void on(std::string name, v8::Persistent<v8::Function> callback) {
      callbacks[name] = callback;
    }

    void call(std::string name)  {
      std::map< std::string, v8::Persistent<v8::Function> >::iterator it;
      it = callbacks.find(name);

      v8::Persistent<v8::Function>* fun = 0;

      //Check if a callback for the given name was found in this object
      if(it != callbacks.end()) {
        //Get the adress of the callback function and send it to the node thread
        //This needs to be the adress from the map element, otherwise we would pass the adress of a local and it fails on the node side.
        fun = &it->second;
      } else {
        //search static callbacks
        it = staticCallbacks.find(name);
        if(it != staticCallbacks.end()) {
          fun = &it->second;
        }
      }
      
      if(fun != 0) {
        NodeCallback* nodeCallback = new NodeCallback();
        nodeCallback->object = this->getV8Object();
        nodeCallback->function = fun;
        asyncHandle->data  = (void*)nodeCallback;
        uv_async_send(asyncHandle);
      }
    };
    protected:
      static void emptySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {};
      uv_async_t* asyncHandle;
      static v8::Persistent<v8::Function> constructor;
      void wait() {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&condition, &mutex);
        pthread_mutex_unlock(&mutex);
      };
      void done() {
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mutex);
      };
    private:
      pthread_mutex_t mutex;
      pthread_cond_t condition;
      std::map<std::string, v8::Persistent<v8::Function> > callbacks;
      static std::map<std::string, v8::Persistent<v8::Function> > staticCallbacks;
};

template <class T> std::map<std::string, v8::Persistent<v8::Function> > SpotifyWrapped<T>::staticCallbacks;
template <class T> v8::Persistent<v8::Function> SpotifyWrapped<T>::constructor;
#endif
