#ifndef _OF_EVENTS
#error "ofEventUtils shouldn't be included directly, include ofEvents.h or ofMain.h"
#endif

#include "ofConstants.h"

#ifdef OF_USING_POCO

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"



//-----------------------------------------
// define ofEvent as a poco FIFOEvent
// to create your own events use:
// ofEvent<argType> myEvent

template <typename ArgumentsType>
class ofEvent: public Poco::FIFOEvent<ArgumentsType> {};

#define OF_ATTENDED_EVENT_EXCEPTION 100

class ofAttendedEventException: public Poco::Exception{
public:
	ofAttendedEventException(): Poco::Exception(OF_ATTENDED_EVENT_EXCEPTION){
		attendedBy = NULL;
	};
	ofAttendedEventException(void * _attendedBy): Poco::Exception(OF_ATTENDED_EVENT_EXCEPTION){
		attendedBy = _attendedBy;
	}

	void * attendedBy;
};



//----------------------------------------------------
// register any method of any class to an event.
// the method must provide one of the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListener(addon.newIntEvent, this, &Class::method)
template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event += Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event += Poco::delegate(listener, listenerMethod);
}


//----------------------------------------------------
// unregister any method of any class to an event.
// the method must provide one the following
// signatures:
//     void method(ArgumentsType & args)
//     void method(const void * sender, ArgumentsType &args)
// ie:
//     ofAddListener(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
static void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&)){
    event -= Poco::delegate(listener, listenerMethod);
}

//----------------------------------------------------
// notifies an event so all the registered listeners
// get called
// ie:
//	ofNotifyEvent(addon.newIntEvent, intArgument, this)
//
// or in case there's no sender:
//	ofNotifyEvent(addon.newIntEvent, intArgument)

template <class EventType,typename ArgumentsType, typename SenderType>
static void * ofNotifyEvent(EventType & event, ArgumentsType & args, SenderType * sender){
	try{
		event.notify(sender,args);
		return NULL;
	}catch(ofAttendedEventException & e){
		return e.attendedBy;
	}
}

template <class EventType,typename ArgumentsType>
static void * ofNotifyEvent(EventType & event, ArgumentsType & args){
	try{
		event.notify(NULL,args);
		return NULL;
	}catch(ofAttendedEventException & e){
		return e.attendedBy;
	}
}


#endif
