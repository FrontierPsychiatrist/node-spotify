#ifndef _SPOTIFY_WRAPPED_H
#define _SPOTIFY_WRAPPED_H

#include <string>
#include <map>
#include <iostream>

#include <uv.h>
#include <node.h>

#include "../NodeCallback.h"
#include "../SpotifyService/SpotifyService.h"

extern SpotifyService* spotifyService;

/**
 * A class used as a base class for wrapping libspotify types to V8 objects.
**/
template <class T>
class SpotifyWrapped : public node::ObjectWrap {
  template <class S> friend class StaticCallbackSetter;
  //friend class Callback<T>;
  typedef void (T::*SimpleMethod)();

  public:
    SpotifyWrapped() : asyncHandle(&spotifyService->callNodeThread), doneCondition(0) {
      pthread_mutex_init(&waitingMutex, NULL);
      pthread_mutex_init(&lockingMutex, NULL);
      pthread_cond_init(&waitingCondition, NULL);
    }; 

    ~SpotifyWrapped() {
      //TODO: unwrap, delete persistent handle
      std::cout << "Destructor called! Please check!" << std::endl;
    };

    /**
     * Get a V8 handle with the Javascript object inside.
     **/
    v8::Handle<v8::Object> getV8Object() {
      //We cannot open a new HandleScope here, as this gets called in the spotify thread!
      //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
      if(handle_.IsEmpty()) {
        v8::Persistent<v8::Object> o = v8::Persistent<v8::Object>::New(constructor->NewInstance());
        this->Wrap(o);
      }
      return handle_;
    }

    /**
     * Save a Javascript callback under a certain name.
     **/
    void on(std::string name, v8::Persistent<v8::Function> callback) {
      callbacks[name] = callback;
    }

    /**
     * Call a Javascript callback by name. The callback will be executed in the nodeJS thread.
     * First, object wide callbacks will be searched, then, class wide callbacks.
     * If no callback is found, nothing happens.
     **/
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
        //Trigger the nodeJS eventloop
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
      pthread_mutex_t lockingMutex;
      static v8::Persistent<v8::Function> constructor;
      
      /**
       * A method to wait for another method in this object to complete, e.g. if it is called in another thread
       * This method locks an object wide mutex, so it can only be called one at a time.
      **/
      void wait() {
        pthread_mutex_lock(&waitingMutex);
        while(!doneCondition) {
          pthread_cond_wait(&waitingCondition, &waitingMutex);
        }
        doneCondition = 0;
        pthread_mutex_unlock(&waitingMutex);
      };
      
      /**
       * Signal the SpotifyWrapped::wait() method that some action is complete.
      **/
      void done() {
        pthread_mutex_lock(&waitingMutex);
        doneCondition = 1;
        pthread_cond_signal(&waitingCondition);
        pthread_mutex_unlock(&waitingMutex);
      };
      
      static v8::Handle<v8::Value> simpleCall(const v8::Arguments& args, SimpleMethod method) {
        v8::HandleScope scope;
        T* obj = node::ObjectWrap::Unwrap<T>(args.This());
        Callback<T>* cb = new Callback<T>(obj, method);
        spotifyService->executeSpotifyAPIcall(cb);
        return scope.Close(v8::Undefined());
      }
    private:
      int doneCondition;
      pthread_mutex_t waitingMutex;
      pthread_cond_t waitingCondition;
      std::map<std::string, v8::Persistent<v8::Function> > callbacks;
      static std::map<std::string, v8::Persistent<v8::Function> > staticCallbacks;
};

template <class T> std::map<std::string, v8::Persistent<v8::Function> > SpotifyWrapped<T>::staticCallbacks;
template <class T> v8::Persistent<v8::Function> SpotifyWrapped<T>::constructor;
#endif
