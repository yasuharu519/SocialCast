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
typedef int Content;

// Comparator

class Event{
    public:
        Vertex fromUser;
        Vertex toUser;
        Content content;
        //
        Event(double _time);
        virtual ~Event();
        //
        double getEventTime() const;
    private:
        double time;
};

class ContentRequestedEvent: public Event{
};

class SendPacketEvent: public Event{
};

class ReceivePacketEvent: public Event{
};


struct DereferenceCompareEvent : public binary_function<Event*, Event*, bool>/*{{{*/
{
    bool operator()(const Event* lhs, const Event* rhs) const
    {
        return lhs->getEventTime() > rhs->getEventTime();
    }
};/*}}}*/

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
    private:
        priority_queue<Event*, vector<Event*>, DereferenceCompareEvent > *eventQ;
        void clearItemsInQueue();
};


#endif
