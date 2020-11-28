#pragma once
//included for debugging reasons
#include <functional>


//simple implementation of C# event type
template <class returnType, class ...params>
class Event {
public:
	//typedef returnType(eventType)(params...);
	typedef std::function<returnType(params...)> eventFuncType;

	struct EventHandler {
		eventFuncType func;
		unsigned int handle;
	};

	Event() {
		
		Size = 10;
		NumSubscribers = 0;
		NumParams = sizeof...(params);
		eventSubcribers = new EventHandler[Size];
	}

	Event(const Event& eventRef) {
		copyEvent(eventRef);
	}

	void copyEvent(const Event& copy) {

		if (this == &copy) {
			return;
		}

		if (eventSubcribers != NULL) {
			DestroyEvent();
		}

		if (copy.eventSubcribers != NULL) {
			Size = copyEvent.Size;
			NumSubscribers = copyEvent.NumSubcribers;

			eventSubcribers = new EventHandler[Size];

			for (int i = 0; i < NumSubscribers; i++) {
				eventSubcribers[i] = copy.eventSubcribers[i];
			}
		}
	}

	~Event() {
		DestroyEvent();
	}

	void DestroyEvent() {
		if (eventSubcribers != NULL) {
			delete[] eventSubcribers;
		}
		Size = NumSubscribers = 0;
	}

	unsigned int operator +=(const eventFuncType& func) {
		if (eventSubcribers == NULL || NumSubscribers == Size) {
			Size *= 2;

			EventHandler* tempHolder = eventSubcribers;
			eventSubcribers = new EventHandler[Size];

			for (int i = 0; NumSubscribers; i++) {
				eventSubcribers = tempHolder;
			}

			delete[] tempHolder;
		}

		unsigned int handle = handleID;
		handleID++;
		eventSubcribers[NumSubscribers].func = func;
		eventSubcribers[NumSubscribers].handle = handle;
		NumSubscribers++;

		return handle;
	}

	const eventFuncType& operator -=(const eventFuncType& func) {
		for (unsigned int i = 0; i < NumSubscribers; i++) {
			if (eventSubcribers[i] == func) {
				NumSubscribers--;
				eventSubcribers[i] = eventSubcribers[NumSubscribers];
				break;
			}
		}

		return func;
	}

	int Find(int handle) {
		for (int i = 0; i < NumSubscribers; i++) {
			if (eventSubcribers[i].handle == handle) {
				return i;
			}
		}
		return -1;
	}

	void Invoke(params... args) {
		for (int i = 0; i < NumSubscribers; i++) {
			eventSubcribers[i].func(args...);
		}
	}

	void operator () (params... args) {
		for (unsigned int i = 0; i < NumSubscribers; i++) {
			eventSubcribers[i].func(args...);
		}
	}

	eventFuncType& operator[](int index) {
		if (index < NumSubscribers) {
			return eventSubcribers[index].func;
		}
		return nullptr;
	}

private:

	EventHandler* eventSubcribers;

	int Size, NumSubscribers, NumParams;

	static int handleID;
};

template <class returnType, class ...params>
int Event<returnType, params ...>::handleID = 0;
