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
    // オプション
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
void Simulator::doSimulation(int contentRequestTime, int ContentCacheSize, bool useProposedMethod)/*{{{*/
{
    // 最初のイベントの追加
    Event* event = new ContentRequestedEvent(0.0);
    eventManager->addEvent(event);
    double time;
    // キャッシュを満たす
    cout << "Filling Cache...";
    physicalNetwork->fillCache(ContentCacheSize, useProposedMethod);
    cout << "  Filled!!" << endl;
    #ifdef DEBUG
    cout << "Loop Start" << endl;
    #endif
    int requestCount = 0;
    while(!(eventManager->isEmpty()))
    {
        event = eventManager->popEvent();
        time = event->getEventTime();
        cout << "Time: " << time << endl;
        // 終了時間を超えていた場合終了
        //if(requestCount >= contentRequestTime){
            //break;
        //}
        // イベント事に振り分け
        if(typeid(*event) == typeid(ContentRequestedEvent))
        {
            // リクエスト回数を超えていた場合終了
            if(requestCount >= contentRequestTime)
                break;
            requestCount++;
            #ifdef DEBUG
            cout << "----------- ContentRequestEvent time:" << time << endl;
            cout << "==========================================" << endl;
            cout << "RequestCount: " << requestCount << endl;
            cout << "==========================================" << endl;
            #endif
            doContentRequest(time, ContentCacheSize, useProposedMethod);
        }
        else if(typeid(*event) == typeid(ContentStartSendingEvent))
        {
            #ifdef DEBUG
            cout << "---------- ContentStartSendingEvent time:" << time << endl;
            #endif
            doContentSending((ContentStartSendingEvent*)event);
        }
        else if(typeid(*event) == typeid(ContentReceivedEvent))
        {
            #ifdef DEBUG
            cout << "---------- ContentReceivedEvent time:" << time << endl;
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

void Simulator::doContentRequest(double _time, int contentCacheSize, bool useProposedMethod)/*{{{*/
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
        // 提案手法での配信
        // 関数の返り値としてパケットIDみたいなのがほしい
        // map<int, vector<int>> みたいなかたちで、packetIDと配信ろがマッチするように
        if(searchFromRequestedUser)
        {
            packetID = physicalNetwork->searchProposedPathFromRequestedUser(requestUserPhysicalID, requestedContentID);
        }
        else
        {
            // TODO: リクエスト元から探索を行わない場合
        }
    }
    else
    {
        // 最短路での配信
        if(searchFromRequestedUser)
        {
            packetID = physicalNetwork->searchPhysicalShortestPathFromRequestedUser(requestUserPhysicalID, requestedContentID);
        }
        else
        {
            // TODO: リクエスト元から探索を行わない場合
        }
    }
    // キャッシュのセット
    VertexList path = physicalNetwork->getPathFromPacketID(packetID);
    //physicalNetwork->setCacheOnRoute(path, requestedContentID, contentCacheSize, useProposedMethod);
    // generateSendPacketEventFromTime(_time, packetID);
    Event* contentSendingEvent = new ContentStartSendingEvent(_time, packetID);
    eventManager->addEvent(contentSendingEvent);
    createNextRequestEvent(_time);
    #ifdef DEBUG
    cout << "doContentRequest: End" << endl;
    #endif
}/*}}}*/

void Simulator::doContentSending(ContentStartSendingEvent* event)/*{{{*/
{
    #ifdef DEBUG
    cout << "doSendPacket: Start" << endl;
    #endif
    // 誰から誰に送るかの情報を得る
    int packetID = event->getPacketID();
    //int sendFrom = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    //int sendTo = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex() + 1);
    VertexList path = physicalNetwork->getPathFromPacketID(packetID);
    cout << "Length: " << path.size() << endl;
    double time = event->getEventTime();
    //#ifdef DEBUG/*{{{*/
    //cout << "Checking whether the line is available..." << endl;
    //cout << "| time: " << time << endl;
    //cout << "| indexOnPath: " << event->getSendFromIndex() << endl;
    //cout << "| packetID: " << packetID << endl;
    //cout << "| sendFrom: " << sendFrom << endl;
    //cout << "| sendTo: " << sendTo << endl;
    //#endif
    // 送り主から送り先が今送っているところか調べる
    // 送っている最中の場合
    //if(physicalNetwork->isSendingTo(sendFrom, sendTo))
    //{
        //#ifdef DEBUG
        //cout << "The line is currently busy..." << endl;
        //#endif
        ////   このイベントをキューに追加
        //eventManager->addSendingEventOnQueue(new SendPacketEvent((*event)), sendFrom, sendTo);
    //}
    //else{/*}}}*/
    // そうでない場合
    //   受信イベントの作成
    //physicalNetwork->setSendingTo(sendFrom, sendTo, true);
    //double receiveTime = time + PACKET_SIZE / BANDWIDTH;
    double receiveTime = time + (CONTENT_SIZE / BANDWIDTH) * (path.size() - 1);
    cout << "===== Time: " << receiveTime - time << endl;
    ContentReceivedEvent *receiveEvent = new ContentReceivedEvent(receiveTime);
    eventManager->addEvent(receiveEvent);
    //}
    #ifdef DEBUG
    cout << "doSendPacket: End" << endl;
    #endif
}/*}}}*/

void Simulator::doContentReceived(ContentReceivedEvent* event)/*{{{*/
{


}/*}}}*/

// パケット関係
void Simulator::doReceivePacket(ReceivePacketEvent* event)/*{{{*/
{
    // 最終的な受信者かどうか調べる
    int packetID = event->getPacketID();
    int receivedUserID = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    int sendUserID = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex() + 1);
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

// private functions
void Simulator::generateSendPacketEventFromTime(double _time, int _packetID)/*{{{*/
{
    #ifdef DEBUG
    cout << "generateSendPacketEventFromTime: Start" << endl;
    #endif
    double nextTime = _time;
    for(int i = 0; i < PACKET_NUM; ++i)
    {
        Event* sendPacketEvent = new SendPacketEvent(nextTime,
                _packetID, i, PACKET_NUM);
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
    int requestCount;
    int cacheSize;
    bool useProposedMethod;
    strcpy(program, argv[0]);
    cout << program << endl;
    if(argc < 4)
    {
        cout << "Usage: " << program << " requestCount cacheSize [method(proposed|not)] " << endl;
        exit(1);
    }
    else
    {
        // 変数の取得
        requestCount = atoi(argv[1]);
        cacheSize = atoi(argv[2]);
        // 提案手法か否か
        if(strcmp(argv[3], "proposed") == 0){
            useProposedMethod = true;
            cout << "useProposedMethod" << endl;
        }else{
            useProposedMethod = false;
            cout << "not useProposedMethod" << endl;
        }

        #ifdef DEBUG
        cout << "StartSimulator" << endl;
        #endif
        Simulator* simulator = new Simulator();
        simulator->doSimulation(requestCount, cacheSize, useProposedMethod);
        delete simulator;
    }
}

