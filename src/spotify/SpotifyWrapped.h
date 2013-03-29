#ifndef _SPOTIFY_WRAPPED_H
#define _SPOTIFY_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>

/**
 * A class used as a base class for wrapping libspotify types to V8 objects.
**/
class SpotifyWrapped : public node::ObjectWrap {
  template <class T> friend class StaticCallbackSetter;

  public:
    SpotifyWrapped(uv_async_t* _handle) : asyncHandle(_handle) {}; 
    v8::Handle<v8::Object> getV8Object();
    /**
     * call a callback by name
     *
     * will first search the callbacks attached to the object, then 
     * the callbacks attached to the class.
     */
    void call(std::string name);

    /**
     * Save a callback for this object
     */
    void on(std::string name, v8::Persistent<v8::Function> callback);

  protected:
    virtual v8::Persistent<v8::Function>& getConstructor() = 0;

    static void emptySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
    uv_async_t* asyncHandle;
  private:
    std::map<std::string, v8::Persistent<v8::Function> > callbacks;
    static std::map<std::string, v8::Persistent<v8::Function> > staticCallbacks;
};

#endif
