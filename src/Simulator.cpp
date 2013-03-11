#include "Simulator.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>


using namespace std;

// Simulator Class
Simulator::Simulator()
{
    evaluationManager = new EvaluationManager();
    relationalGraph = new RelationalGraph();
    physicalNetwork = new PhysicalNetwork(relationalGraph, evaluationManager);
    eventManager = new EventManager();
    useProposedMethod = false;
    searchFromRequestedUser = true;
}

Simulator::~Simulator()
{
    delete evaluationManager;
    delete relationalGraph;
    delete physicalNetwork;
    delete eventManager;
}

// public functions
void Simulator::doSimulation(double endTime)/*{{{*/
{
    // 最初のイベントの追加
    Event* event = new ContentRequestedEvent(0.0);
    eventManager->addEvent(event);
    double time;
    #ifdef DEBUG
    cout << "Loop Start" << endl;
    #endif
    while(!(eventManager->isEmpty()))
    {
        event = eventManager->popEvent();
        time = event->getEventTime();
        // 終了時間を超えていた場合終了
        if(time > endTime){
            break;
        }
        // イベント事に振り分け
        if(typeid(*event) == typeid(ContentRequestedEvent))
        {
            #ifdef DEBUG
            cout << "ContentRequestEvent time:" << time << endl;
            #endif
            doContentRequest(time);
        }
        else if(typeid(*event) == typeid(ReceivePacketEvent))
        {
            #ifdef DEBUG
            cout << "ReceivePacketEvent time:" << time << endl;
            #endif
            doReceivePacket((ReceivePacketEvent*)event);
        }
        else if(typeid(*event) == typeid(SendPacketEvent))
        {
            #ifdef DEBUG
            cout << "SendPacketEvent time:" << time << endl;
            #endif
            doSendPacket((SendPacketEvent*)event);
        }
        else if(typeid(*event) == typeid(ContentReceivedEvent))
        {
            #ifdef DEBUG
            cout << "ContentReceivedEvent time:" << time << endl;
            #endif
            doContentReceived((ContentReceivedEvent*)event);
        }
        else{
            cout << "Error: Event class should not be in eventManager" << endl;
            std::abort();
        }
        delete event;
    }
}/*}}}*/

void Simulator::createNextRequestEvent(double _time)/*{{{*/
{
    #ifdef DEBUG
    cout << "createNextRequsetEvent: Start" << endl;
    #endif
    // boost使うよ!!
    using namespace boost;
    // 変数
    double result;
    // ランダムの設定
    mt19937 gen(static_cast<unsigned long>(time(0)) );
    uniform_real<> dist( 0, 1);
    variate_generator<mt19937&, uniform_real<> > rand( gen, dist );
    result = -1.0 / LAMBDA * log(1.0 - rand());
    // Eventの作成
    Event* newRequest = new ContentRequestedEvent(_time + result);
    // Eventの追加
    eventManager->addEvent(newRequest);
    #ifdef DEBUG
    cout << "createNextRequsetEvent: End" << endl;
    #endif
}/*}}}*/

void Simulator::doContentRequest(double _time)/*{{{*/
{
    #ifdef DEBUG
    cout << "doContentRequest: Start" << endl;
    #endif
    Vertex requestUserPhysicalID = physicalNetwork->chooseRequestUser();
    ContentID requestedContentID = physicalNetwork->chooseRequestContent(requestUserPhysicalID);
    #ifdef DEBUG
    cout << "UserPhysicalID: " << requestUserPhysicalID << " ContentID: " << requestedContentID << endl;
    #endif
    int packetID;
    if(useProposedMethod)
    {
        // TODO: 提案手法での配信
        // 関数の返り値としてパケットIDみたいなのがほしい
        // map<int, vector<int>> みたいなかたちで、packetIDと配信ろがマッチするように
        if(searchFromRequestedUser)
        {
            // TODO: リクエストのユーザに最も近いところからの経路を探索
        }
        else
        {
            // TODO: 経路に無ければ、みたいな
        }
        packetID = 0;
    }
    else
    {
        // TODO: 最短路での配信
        if(searchFromRequestedUser)
        {
            // TODO: リクエストのユーザに最も近いところからの経路を探索
        }
        else
        {
            // TODO: 経路に無ければ、みたいな
        }
        packetID = 0;
    }
    generateSendPacketEventFromTime(_time, packetID);
    createNextRequestEvent(_time);
    #ifdef DEBUG
    cout << "doContentRequest: End" << endl;
    #endif
}/*}}}*/

void Simulator::doReceivePacket(ReceivePacketEvent* event)/*{{{*/
{
    // 最終的な受信者かどうか調べる
    int packetID = event->getPacketID();
    int receivedUserID = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    int sendUserID = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex() - 1);
    int packetIndex = event->getPacketIndex();
    int packetSum = event->getPacketSum();
    double time = event->getEventTime();
    if(physicalNetwork->isLastUserToReceivePacket(packetID, receivedUserID))
    {
    // 最終的な受信者の場合
    //   そのコンテンツに対するパケットの数と受信したパケットの数が一致
    //     コンテンツ受信
    //   一致しない場合
    //     何もしない
        if(packetIndex == (packetSum - 1))
        {
            ContentReceivedEvent *contentReceivedEvent = new ContentReceivedEvent(time);
            eventManager->addEvent(contentReceivedEvent);
            // TODO: 要らないデータなどの消去
        }
    }
    else
    {
    // 最終的な受信者でないばあい
    //   次の人へと送信イベントを起こす
        SendPacketEvent* nextSendPacketEvent = (ReceivePacketEvent*)event->getNextSendPacketEvent();
        eventManager->addEvent(nextSendPacketEvent);
    }
    // キューを確認、このイベント発生後にキューに溜まっているものがあれば、そのイベントを発生させる
    if(!(eventManager->isSendingEventQueueEmpty(sendUserID, receivedUserID)))
    {
        SendPacketEvent* newEvent = eventManager->popSendingEventFromQueue(sendUserID, receivedUserID);
        newEvent->setEventTime(time);
        eventManager->addEvent(newEvent);
    }
}/*}}}*/

void Simulator::doSendPacket(SendPacketEvent* event)/*{{{*/
{
    // 誰から誰に送るかの情報を得る
    int packetID = event->getPacketID();
    int sendFrom = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    event->incrementSendFromIndex();
    int sendTo = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    double time = event->getEventTime();
    // 送り主から送り先が今送っているところか調べる
    // 送っている最中の場合
    if(physicalNetwork->isSendingTo(sendFrom, sendTo))
    {
        //   このイベントをキューに追加
        eventManager->addSendingEventOnQueue(new SendPacketEvent((*event)), sendFrom, sendTo);
    }
    else{
    // そうでない場合
    //   受信イベントの作成
        physicalNetwork->setSendingTo(sendFrom, sendTo, true);
        double receiveTime = time + PACKET_SIZE / BANDWIDTH;
        ReceivePacketEvent *receiveEvent = new ReceivePacketEvent(receiveTime, event);
        eventManager->addEvent(receiveEvent);
    }
}/*}}}*/

void Simulator::doContentReceived(ContentReceivedEvent* event)/*{{{*/
{

}/*}}}*/

// private functions
void Simulator::generateSendPacketEventFromTime(double _time, int _packetID)/*{{{*/
{
    #ifdef DEBUG
    cout << "generateSendPacketEventFromTime: Start" << endl;
    #endif
    double nextTime = _time;
    for(int i = 0; i < PACKET_NUM; ++i)
    {
        Event* sendPacketEvent = new SendPacketEvent(nextTime, _packetID, i, PACKET_NUM);
        eventManager->addEvent(sendPacketEvent);
        nextTime += PACKET_KANKAKU;
    }
    #ifdef DEBUG
    cout << "generateSendPacketEventFromTime: End" << endl;
    #endif
}/*}}}*/

int main(int argc, char* argv[])
{
    char program[256];
    char method[256];
    strcpy(program, argv[0]);
    cout << program << endl;
    if(argc < 2)
    {
        cout << "Usage: " << program << " [method] " << endl;
        exit(1);
    }
    else
    {
        #ifdef DEBUG
        cout << "StartSimulator" << endl;
        #endif
        Simulator* simulator = new Simulator();
        simulator->doSimulation(10000.0);
        delete simulator;
    }
}

