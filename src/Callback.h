template <class T>
class Callback {
	public:
		void call() {
			object->*method();
		};
		Callback(T* _object, void(T::*_method)()) : object(_object), method(_method) {};
	private:
		T* object;
		void(T::*method)();
};
