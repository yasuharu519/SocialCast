#include "gtest/gtest.h"
#include "EventManager.hpp"

class EventManagerTest: public ::testing::Test{
    public:
        EventManager* eventManager = new EventManager();
};


//getEdgeWeight
TEST_F(EventManagerTest, TestingPush){/*{{{*/
    EXPECT_EQ(0, eventManager->size());
    Event* event1 = new Event(100.0);
    eventManager->addEvent(event1);
    EXPECT_EQ(1, eventManager->size());
}/*}}}*/

TEST_F(EventManagerTest, TestingPush2){/*{{{*/
    EXPECT_EQ(0, eventManager->size());
    Event* event1 = new Event(100.0);
    Event* event2 = new Event(10.0);
    Event* event3 = new Event(1.0);
    Event* event4 = new Event(20.0);
    Event* event5 = new Event(101.0);
    eventManager->addEvent(event1);
    eventManager->addEvent(event2);
    eventManager->addEvent(event3);
    eventManager->addEvent(event4);
    eventManager->addEvent(event5);
    EXPECT_EQ(5, eventManager->size());
}/*}}}*/

TEST_F(EventManagerTest, popEvnetTest)/*{{{*/
{
    Event* event1 = new Event(100.0);
    Event* event2 = new Event(10.0);
    Event* event3 = new Event(1.0);
    Event* event4 = new Event(20.0);
    Event* event5 = new Event(101.0);
    eventManager->addEvent(event1);
    eventManager->addEvent(event2);
    eventManager->addEvent(event3);
    eventManager->addEvent(event4);
    eventManager->addEvent(event5);
    EXPECT_EQ(event3, eventManager->popEvent());
    delete event3;
    EXPECT_EQ(event2, eventManager->popEvent());
    delete event2;
    EXPECT_EQ(event4, eventManager->popEvent());
    delete event4;
    EXPECT_EQ(event1, eventManager->popEvent());
    delete event1;
    EXPECT_EQ(event5, eventManager->popEvent());
    delete event5;
}/*}}}*/

TEST_F(EventManagerTest, clearTest)/*{{{*/
{
    EXPECT_EQ(0, eventManager->size());
    Event* event1 = new Event(100.0);
    eventManager->addEvent(event1);
    EXPECT_EQ(1, eventManager->size());
    eventManager->clear();
    EXPECT_EQ(0, eventManager->size());
}/*}}}*/

TEST_F(EventManagerTest, isEmptyTest)/*{{{*/
{
    EXPECT_EQ(true, eventManager->isEmpty());
    Event* event1 = new Event(100.0);
    eventManager->addEvent(event1);
    EXPECT_EQ(false, eventManager->isEmpty());
    eventManager->clear();
    EXPECT_EQ(true, eventManager->isEmpty());
}/*}}}*/
