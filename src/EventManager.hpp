#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__

//#include "boost/tuple/tuple.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <climits>
#include <queue>
#define foreach BOOST_FOREACH

class Event{
    public:
        Event();
        virtual ~Event();
};

class ContentRequestedEvent: public Event{
    public:
        ContentRequestedEvent();
        virtual ~ContentRequestedEvent();
};

class SendPacketEvent: public Event{
    public:
        SendPacketEvent();
        virtual ~SendPacketEvent();
};

class ReceivePacketEvent: public Event{
    public:
        ReceivePacketEvent();
        virtual ~ReceivePacketEvent();
};

class EventManager{
    public:
        EventManager();
        virtual ~EventManager();
};

#endif
