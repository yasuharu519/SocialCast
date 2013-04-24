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
    //cout << "UserNum: " << relationalGraph->getUserList().size() << endl;
    //cout << "ContentNum: " << relationalGraph->getContentList().size() << endl;
}

Simulator::~Simulator()
{
    delete evaluationManager;
    delete relationalGraph;
    delete physicalNetwork;
    delete eventManager;
}

void Simulator::doSimulation(int contentRequestTime, int ContentCacheSize, bool useProposedMethod,
        ofstream &distribution_datafile, ofstream &path_datafile)/*{{{*/
{
    // 最初のイベントの追加
    Event* event = new ContentRequestedEvent(0.0);
    eventManager->addEvent(event);
    double time;
    // キャッシュを満たす
    #ifdef DEBUG
    cout << "Loop Start" << endl;
    #endif
    int requestCount = 0;
    while(!(eventManager->isEmpty()))
    {
        event = eventManager->popEvent();
        time = event->getEventTime();
        #ifdef DEBUG
        cout << "Time: " << time << endl;
        #endif
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
            doContentSending((ContentStartSendingEvent*)event,
                    distribution_datafile, path_datafile);
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

void Simulator::doContentRequest(double _time, int contentCacheSize, bool useProposedMethod)
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
    Event* contentSendingEvent = new ContentStartSendingEvent(_time, packetID);
    eventManager->addEvent(contentSendingEvent);
    createNextRequestEvent(_time);
    #ifdef DEBUG
    cout << "doContentRequest: End" << endl;
    #endif
}/*}}}*/

double Simulator::calcRelationalStrengthOfPath(VertexList path, int contentID)
{
    double result = 0.0;
    double tmp;
    VertexList::iterator it;
    int relationalID;
    int count = 0;
    for(it = path.begin(); it != path.end(); ++it)
    {
        relationalID = physicalNetwork->physicalToRelational[(*it)];
        if(relationalID != physicalNetwork->distributorID){
            tmp = relationalGraph->dijkstraShortestPathLength(relationalID, contentID);
            result += 1.0 / tmp;
            count++;
        }
    }
    return result / count;
}

void Simulator::doContentSending(ContentStartSendingEvent* event,
                ofstream &distribution_datafile, ofstream &path_datafile)
{
    #ifdef DEBUG
    cout << "doSendPacket: Start" << endl;
    #endif
    // 誰から誰に送るかの情報を得る
    int packetID = event->getPacketID();
    int contentID = physicalNetwork->getContentIDFromPacketID(packetID);
    //int sendFrom = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex());
    //int sendTo = physicalNetwork->getUserOnPathIndexWithPacketID(packetID, event->getSendFromIndex() + 1);
    VertexList path = physicalNetwork->getPathFromPacketID(packetID);
    #ifdef DEBUG
    cout << "Length: " << path.size() << endl;
    #endif
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
    // 書き込み
    distribution_datafile << receiveTime - time << ", " << calcRelationalStrengthOfPath(path, contentID) << endl;
    if(path.size() != 0){
        path_datafile << path[0];
        if(path.size() > 1){
            for(int i = 1; i < path.size(); ++i){
                path_datafile << ", ";
                path_datafile << path[i];
            }
        }
    }
    path_datafile << endl;
        
    #ifdef DEBUG
    cout << "===== Time: " << receiveTime - time << endl;
    #endif
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

// キャッシュ関連
void Simulator::resetCacheOfNodes()
{
    physicalNetwork->clearCacheOfNodes();
}

void Simulator::setCacheBasedOnMethod(bool useProposedMethod, int cacheSize)
{
    physicalNetwork->fillCache(cacheSize, useProposedMethod);
}

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
    ofstream distribution_datafile;
    ofstream path_datafile;
    int requestCount;
    int cacheSize;
    bool useProposedMethod;
    strcpy(program, argv[0]);
    if(argc < 3)
    {
        cout << "Usage: " << program << " requestCount cacheSize" << endl;
        exit(1);
    }
    else
    {
        // 変数の取得
        requestCount = atoi(argv[1]);
        cacheSize = atoi(argv[2]);

        #ifdef DEBUG
        cout << "StartSimulator" << endl;
        #endif
        //// 提案手法での配信
        useProposedMethod = true;
        distribution_datafile.open("LatencyAndSumOfSocialMetric_proposed.txt");
        path_datafile.open("path_proposed.txt");
        Simulator *simulator = new Simulator();
        simulator->setCacheBasedOnMethod(useProposedMethod, cacheSize);
        simulator->doSimulation(requestCount, cacheSize, useProposedMethod, distribution_datafile, path_datafile);
        distribution_datafile.close();
        path_datafile.close();
        // クリア
        simulator->resetCacheOfNodes();
        // 比較手法での配信
        useProposedMethod = false;
        distribution_datafile.open("LatencyAndSumOfSocialMetric_conventional.txt");
        path_datafile.open("path_conventional.txt");
        simulator->setCacheBasedOnMethod(useProposedMethod, cacheSize);
        simulator->doSimulation(requestCount, cacheSize, useProposedMethod, distribution_datafile, path_datafile);
        distribution_datafile.close();
        path_datafile.close();
        delete simulator;

    }
}

