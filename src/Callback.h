#ifndef CALLBACK_H
#define CALLBACK_H
/**
 * Helper class for abstracting callbacks.
 *
 * This basically wraps an object and a method of its class and
 * provides the functionality to call the method on the object.
 *
 * The base class CallbackBase is needed if the type parameter T is
 * not known or uninteresting at compile time
 *
 * Usage:
 * class Printer {
 *  public:
 *    void print() {
 *      fprintf(stdout, "Hello World\n");
 *    }
 * }
 *
 * ...
 * Printer* printer = new Printer();
 * Callback<Printer>* callback = new Callback<Printer>(printer, &Printer::print);
 *
 * callback->call();
 *
 * delete callback;
 * delete printer;
 * **/
class CallbackBase {
  public:
    virtual void call() = 0;
    virtual ~CallbackBase() {};
};

template <class T>
class Callback : public CallbackBase {
  public:
    void call() {
      (object->*method)();
    };
    Callback(T* _object, void(T::*_method)()) : object(_object), method(_method) {};
    ~Callback() {};
  private:
    T* object; //Object to call method on
    void(T::*method)(); //Method to call
};
#endif
