#include <iostream>
class CallbackBase {
	public:
		virtual void call();
};

template <class T>
class Callback : public CallbackBase {
	public:
		void call() {
			(object->*method)();
		};
		Callback(T* _object, void(T::*_method)()) : object(_object), method(_method) {};
	private:
		T* object;
		void(T::*method)();
};
