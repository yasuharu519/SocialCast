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
Event::Event(const Event& other)
{
    time = other.time;
}

//Public
double Event::getEventTime() const
{
    return time;
}

void Event::setEventTime(double _time)
{
    time = _time;
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
SendPacketEvent::SendPacketEvent(double _time, int _packetID, int _packetIndex, int _packetSum):Event(_time){
    packetIndex = _packetIndex;
    packetSum = _packetSum;
    packetID = _packetID;
    sendFromIndex = 0;
}

SendPacketEvent::SendPacketEvent(const SendPacketEvent& rhs):Event(rhs){
    packetIndex = rhs.packetIndex;
    packetSum = rhs.packetSum;
    packetID = rhs.packetID;
    sendFromIndex = rhs.sendFromIndex;
}

SendPacketEvent::~SendPacketEvent(){

}

int SendPacketEvent::getPacketIndex()
{
    return packetIndex;
}
int SendPacketEvent::getSendFromIndex()
{
    return sendFromIndex;
}
int SendPacketEvent::getPacketID()
{
    return packetID;
}
int SendPacketEvent::getPacketSum()
{
    return packetSum;
}
void SendPacketEvent::incrementSendFromIndex()
{
    ++sendFromIndex;
}

// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// ReceivePacketEvent
///////////////////////////////////////////////////////////////////////////////
ReceivePacketEvent::ReceivePacketEvent(double time, SendPacketEvent *event):SendPacketEvent(time, 0, 0, 0){
    packetIndex = event->getPacketIndex();
    packetSum = event->getPacketSum();
    packetID = event->getPacketID();
    sendFromIndex = event->getSendFromIndex();
    savedSendPacketEvent = new SendPacketEvent((*event));
}

ReceivePacketEvent::~ReceivePacketEvent(){
}

SendPacketEvent* ReceivePacketEvent::getNextSendPacketEvent(){
    return savedSendPacketEvent;
}

void ReceivePacketEvent::deleteNextSendPacketEvent(){
    delete savedSendPacketEvent;
}

// ------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// ContentReceivedEvent
///////////////////////////////////////////////////////////////////////////////
ContentReceivedEvent::ContentReceivedEvent(double _time):Event(_time)
{

}

ContentReceivedEvent::~ContentReceivedEvent()
{
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

void EventManager::addSendingEventOnQueue(SendPacketEvent* sendPacketEvent, int from, int to)/*{{{*/
{
    map<int, SendingPacketEventQueueMap*>::iterator outerIterator = sendingPacketEventQueueMapMap.find(from);
    SendingPacketEventQueueMap* _map;
    SendingPacketEventQueueMap::iterator innerIterator;
    SendingPacketEventQueue* _queue;
    if(outerIterator == sendingPacketEventQueueMapMap.end())
    {
        sendingPacketEventQueueMapMap[from] = new SendingPacketEventQueueMap();
    }
    _map = sendingPacketEventQueueMapMap[from];
    innerIterator = _map->find(to);
    if(innerIterator == _map->end())
    {
        (*_map)[to] = new SendingPacketEventQueue();
    }
    _queue = _map->at(to);
    _queue->push(sendPacketEvent);
}/*}}}*/

bool EventManager::isSendingEventQueueEmpty(int from, int to)/*{{{*/
{
    map<int, SendingPacketEventQueueMap*>::iterator outerIterator = sendingPacketEventQueueMapMap.find(from);
    SendingPacketEventQueueMap::iterator innerIterator;
    SendingPacketEventQueueMap* _map;
    SendingPacketEventQueue* _queue;
    if(outerIterator == sendingPacketEventQueueMapMap.end())
    {
        sendingPacketEventQueueMapMap[from] = new SendingPacketEventQueueMap();
    }
    _map = sendingPacketEventQueueMapMap[from];
    innerIterator = _map->find(to);
    if(innerIterator == _map->end())
    {
         (*_map)[to] = new SendingPacketEventQueue();
    }
    _queue = _map->at(to);
    return _queue->empty();
}/*}}}*/

SendPacketEvent* EventManager::popSendingEventFromQueue(int from, int to)/*{{{*/
{
    SendingPacketEventQueueMap *_map = sendingPacketEventQueueMapMap[from];
    SendingPacketEventQueue* _queue = _map->at(to);
    SendPacketEvent* event = _queue->top();
    _queue->pop();
    return event;
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

