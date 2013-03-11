#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__

#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <climits>
#include <queue>
#include <list>
#include <functional>
#define foreach BOOST_FOREACH

using namespace std;

typedef int Vertex;
typedef int ContentID;


class Event{
    public:
        Event(double _time);
        virtual ~Event();
        Event(const Event& other);
        //
        double getEventTime() const;
        void setEventTime(double _time);
    private:
        double time;
};

class ContentRequestedEvent: public Event{
    public:
        ContentRequestedEvent(double _time);
        ~ContentRequestedEvent();
};

class SendPacketEvent: public Event{
    public:
        SendPacketEvent(double _time, int _packetID, int _packetIndex, int _packetSum );
        ~SendPacketEvent();
        // コピーコンストラクタ
        SendPacketEvent(const SendPacketEvent& rhs);
        //
        int getPacketIndex();
        int getSendFromIndex();
        int getPacketID();
        int getPacketSum();
        void incrementSendFromIndex();
    protected:
        int packetIndex;
        int packetID;
        int sendFromIndex;
        int packetSum;
};

class ReceivePacketEvent: public SendPacketEvent{
    public:
        ReceivePacketEvent(double time, SendPacketEvent *event);
        ~ReceivePacketEvent();
        SendPacketEvent* getNextSendPacketEvent();
        void deleteNextSendPacketEvent();
    private:
        SendPacketEvent *savedSendPacketEvent;
};

class ContentReceivedEvent: public Event{
    public:
        ContentReceivedEvent(double _time);
        ~ContentReceivedEvent();
};

// Comparator
struct DereferenceCompareEvent : public binary_function<Event*, Event*, bool>/*{{{*/
{
    bool operator()(const Event* lhs, const Event* rhs) const
    {
        return lhs->getEventTime() > rhs->getEventTime();
    }
};/*}}}*/

typedef priority_queue<SendPacketEvent*, vector<SendPacketEvent*>, DereferenceCompareEvent > SendingPacketEventQueue;
typedef map<int, SendingPacketEventQueue* > SendingPacketEventQueueMap;

class EventManager{
    public:
        // Constructor/destructor
        EventManager();
        virtual ~EventManager();
        // funcs
        void addEvent(Event* event);
        Event* popEvent();
        void clear();
        int size();
        bool isEmpty();
        void addSendingEventOnQueue(SendPacketEvent* sendPacketEvent, int from, int to);
        bool isSendingEventQueueEmpty(int from, int to);
        SendPacketEvent* popSendingEventFromQueue(int from, int to);
    private:
        priority_queue<Event*, vector<Event*>, DereferenceCompareEvent > *eventQ;
        void clearItemsInQueue();
        map<int, SendingPacketEventQueueMap*> sendingPacketEventQueueMapMap;
};


#endif
