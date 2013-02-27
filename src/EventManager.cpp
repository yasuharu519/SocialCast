#include "EventManager.hpp"

// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// Event
///////////////////////////////////////////////////////////////////////////////
Event::Event(double _time){
    time = _time;
}

Event::~Event(){

}

//Public
double Event::getEventTime() const
{
    return time;
}

// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// ContentRequestedEvent
///////////////////////////////////////////////////////////////////////////////
ContentRequestedEvent::ContentRequestedEvent(double _time):Event(_time){

}

ContentRequestedEvent::~ContentRequestedEvent(){

}


// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// SendPacketEvent
///////////////////////////////////////////////////////////////////////////////
SendPacketEvent::SendPacketEvent(double _time):Event(_time){

}

SendPacketEvent::~SendPacketEvent(){

}


// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// ReceivePacketEvent
///////////////////////////////////////////////////////////////////////////////
ReceivePacketEvent::ReceivePacketEvent(double _time):Event(_time){

}

ReceivePacketEvent::~ReceivePacketEvent(){

}

// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// EventManager
///////////////////////////////////////////////////////////////////////////////
EventManager::EventManager(){/*{{{*/
    eventQ = new priority_queue<Event*, vector<Event*>, DereferenceCompareEvent >();
}/*}}}*/

EventManager::~EventManager(){/*{{{*/
    clearItemsInQueue();
    delete eventQ;
}/*}}}*/

//Public
void EventManager::addEvent(Event* event)/*{{{*/
{
    eventQ->push(event);
}/*}}}*/

Event* EventManager::popEvent()/*{{{*/
{
    Event* poped = eventQ->top();
    eventQ->pop();
    return poped;
}/*}}}*/

void EventManager::clear()/*{{{*/
{
    clearItemsInQueue();
    delete eventQ;
    eventQ = new priority_queue<Event*, vector<Event*>, DereferenceCompareEvent >();
}/*}}}*/

int EventManager::size()/*{{{*/
{
    return eventQ->size();
}/*}}}*/

bool EventManager::isEmpty()/*{{{*/
{
    return eventQ->empty() == true;
}/*}}}*/

// private
void EventManager::clearItemsInQueue()/*{{{*/
{
    while(!eventQ->empty())
    {
        Event* e = eventQ->top();
        eventQ->pop();
        delete e;
    }
}/*}}}*/

