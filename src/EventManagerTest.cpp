#include "gtest/gtest.h"
#include "EventManager.hpp"

class EventManagerTest: public ::testing::Test{
    public:
        EventManagerTest();
        ~EventManagerTest();
        EventManager* eventManager;
};

EventManagerTest::EventManagerTest()
{
    eventManager = new EventManager();
}

EventManagerTest::~EventManagerTest()
{
    delete eventManager;

}


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
    EXPECT_NE(true, eventManager->isEmpty());
    //EXPECT_EQ(1, eventManager->isEmpty());
    eventManager->clear();
    EXPECT_EQ(true, eventManager->isEmpty());
}/*}}}*/

TEST_F(EventManagerTest, addSendingEventOnQueue)/*{{{*/
{
    SendPacketEvent *event1 = new SendPacketEvent(1.0, 1, 1, 5);
    SendPacketEvent *event2 = new SendPacketEvent(2.0, 1, 2, 5);
    SendPacketEvent *event3 = new SendPacketEvent(3.0, 1, 3, 5);
    SendPacketEvent *event4 = new SendPacketEvent(4.0, 1, 4, 5);
    SendPacketEvent *event5 = new SendPacketEvent(5.0, 1, 5, 5);
    eventManager->addSendingEventOnQueue(event1, 1, 2);
    eventManager->addSendingEventOnQueue(event2, 1, 2);
    eventManager->addSendingEventOnQueue(event3, 1, 2);
    eventManager->addSendingEventOnQueue(event4, 1, 2);
    eventManager->addSendingEventOnQueue(event5, 1, 2);
    delete event1;
    delete event2;
    delete event3;
    delete event4;
    delete event5;
}/*}}}*/

TEST_F(EventManagerTest, isSendingEventQueueEmpty)/*{{{*/
{
    SendPacketEvent *event1 = new SendPacketEvent(1.0, 1, 1, 5);
    SendPacketEvent *event2 = new SendPacketEvent(2.0, 1, 2, 5);
    EXPECT_EQ(true, eventManager->isSendingEventQueueEmpty(1, 2));
    eventManager->addSendingEventOnQueue(event1, 1, 2);
    EXPECT_NE(true, eventManager->isSendingEventQueueEmpty(1, 2));
    EXPECT_EQ(true, eventManager->isSendingEventQueueEmpty(2, 1));
    eventManager->addSendingEventOnQueue(event2, 2, 1);
    delete event1;
    delete event2;
}/*}}}*/

TEST_F(EventManagerTest, popSendingEventFromQueue1)/*{{{*/
{
    SendPacketEvent *event1 = new SendPacketEvent(1.0, 1, 1, 5);
    SendPacketEvent *event2 = new SendPacketEvent(2.0, 1, 2, 5);
    SendPacketEvent *event3 = new SendPacketEvent(3.0, 1, 3, 5);
    SendPacketEvent *event4 = new SendPacketEvent(4.0, 1, 4, 5);
    SendPacketEvent *event5 = new SendPacketEvent(5.0, 1, 5, 5);
    eventManager->addSendingEventOnQueue(event1, 1, 2);
    eventManager->addSendingEventOnQueue(event2, 1, 2);
    eventManager->addSendingEventOnQueue(event3, 1, 2);
    eventManager->addSendingEventOnQueue(event4, 1, 2);
    eventManager->addSendingEventOnQueue(event5, 1, 2);
    EXPECT_EQ(event1, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event2, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event3, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event4, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event5, eventManager->popSendingEventFromQueue(1, 2));
    delete event1;
    delete event2;
    delete event3;
    delete event4;
    delete event5;
}/*}}}*/

TEST_F(EventManagerTest, popSendingEventFromQueue2)/*{{{*/
{
    SendPacketEvent *event1 = new SendPacketEvent(1.0, 1, 1, 5);
    SendPacketEvent *event2 = new SendPacketEvent(100.0, 1, 2, 5);
    SendPacketEvent *event3 = new SendPacketEvent(10.0, 1, 3, 5);
    SendPacketEvent *event4 = new SendPacketEvent(30.0, 1, 4, 5);
    SendPacketEvent *event5 = new SendPacketEvent(20.0, 1, 5, 5);
    eventManager->addSendingEventOnQueue(event1, 1, 2);
    eventManager->addSendingEventOnQueue(event2, 1, 2);
    eventManager->addSendingEventOnQueue(event3, 1, 2);
    eventManager->addSendingEventOnQueue(event4, 1, 2);
    eventManager->addSendingEventOnQueue(event5, 1, 2);
    EXPECT_EQ(event1, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event3, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event5, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event4, eventManager->popSendingEventFromQueue(1, 2));
    EXPECT_EQ(event2, eventManager->popSendingEventFromQueue(1, 2));
    delete event1;
    delete event2;
    delete event3;
    delete event4;
    delete event5;
}/*}}}*/

TEST_F(EventManagerTest, sendPacketCopyConstructorTest)
{
    SendPacketEvent* event1 = new SendPacketEvent(1.0, 1, 0, 2);
    SendPacketEvent* event2 = new SendPacketEvent((*event1));
    EXPECT_EQ(1.0, event1->getEventTime());
    EXPECT_EQ(1.0, event2->getEventTime());
    event2->setEventTime(2.0);
    EXPECT_EQ(1.0, event1->getEventTime());
    EXPECT_EQ(2.0, event2->getEventTime());
    delete event1;
    delete event2;
}


TEST_F(EventManagerTest, ReceivePacketConstructorTest)
{
    SendPacketEvent* event1 = new SendPacketEvent(1.0, 1, 0, 2);
    ReceivePacketEvent* event2 = new ReceivePacketEvent(2.0, event1);
    EXPECT_EQ(1.0, event1->getEventTime());
    EXPECT_EQ(2.0, event2->getEventTime());
    SendPacketEvent* event3 = event2->getNextSendPacketEvent();
    delete event2;
    EXPECT_NE(event3, event1);
    event3->setEventTime(3.0);
    EXPECT_NE(3.0, event1->getEventTime());
    EXPECT_EQ(3.0, event3->getEventTime());
    delete event1;
    delete event3;
}
