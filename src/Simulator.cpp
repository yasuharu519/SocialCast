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
}

Simulator::~Simulator()
{
    delete evaluationManager;
    delete relationalGraph;
    delete physicalNetwork;
    delete eventManager;
}

// public functions
void Simulator::doSimulation()
{
    // 最初のイベントの追加
    Event* event = new ContentRequestedEvent(0.0);
    double time;
    while(!(eventManager->isEmpty()))
    {
        event = eventManager->popEvent();
        time = event->getEventTime();
        if(typeid(event) == typeid(ContentRequestedEvent))
        {
            doContentRequest(time);
        }
        else if(typeid(event) == typeid(ReceivePacketEvent))
        {
            doReceivePacket(ReceivePacketEvent* event);
        }
        else if(typeid(event) == typeid(SendPacketEvent))
        {
            doSendPacket(SendPacketEvent* event);
        }
        else{
            cout << "Error: Event class should not be in eventManager" << endl;
            std::abort();
        }
        delete event;
    }
}

void Simulator::createNextRequestEvent(double _time)
{
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
}

void Simulator::doContentRequest(double _time)
{
    Vertex requestUserPhysicalID = physicalNetwork->chooseRequestUser();
    ContentID requestedContentID = physicalNetwork->chooseRequestContent(requestUserPhysicalID);
    int packetID;
    if(useProposedMethod)
    {
        // TODO: 比較手法での配信
        // 関数の返り値としてパケットIDみたいなのがほしい
        // map<int, vector<int>> みたいなかたちで、packetIDと配信ろがマッチするように
        packetID = 0;
    }
    else
    {
        // TODO: 最短路での配信
        packetID = 0;
    }
    generateSendPacketEventFromTime(_time, packetID);
    createNextRequestEvent(_time);
}

void Simulator::doReceivePacket(ReceivePacketEvent* event)
{
    // 最終的な受信者かどうか調べる
    // 最終的な受信者でないばあい
    //   indexを増やす
    //   次の人へと送信イベントを起こす
    // 最終的な受信者の場合
    //   そのコンテンツに対するパケットの数と受信したパケットの数が一致
    //     コンテンツ受信
    //   一致しない場合
    //     何もしない

    // TODO: 次のsendPacketEvent作成
    //Event* newSendEvent = 

    // キューを確認、このイベント発生後にキューに溜まっているものがあれば、そのイベントを発生させる

}

void Simulator::doSendPacket(SendPacketEvent* event)
{
    // 誰から誰に送るかの情報を得る
    int sendFrom = event->getSendFromIndex();
    event->incrementSendFromIndex();
    int sendTo = event->getSendFromIndex();
    // 送り主から送り先が今送っているところか調べる
    // 送っている最中の場合
    if(physicalNetwork->isSendingTo(sendFrom, sendTo))
    {
        //   このイベントをキューに追加
        eventManager->addSendingEventOnQueue(event, sendFrom, sendTo);
        
    }
    // そうでない場合
    //   受信イベントの作成
}

// private functions
void Simulator::generateSendPacketEventFromTime(double _time, int _packetID)/*{{{*/
{
    double nextTime = _time;
    for(int i = 0; i < PACKET_NUM; ++i)
    {
        Event* sendPacketEvent = new SendPacketEvent(nextTime, _packetID, i, PACKET_NUM);
        eventManager->addEvent(sendPacketEvent);
        nextTime += PACKET_KANKAKU;
    }
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
        Simulator* simulator = new Simulator();
        simulator->doSimulation();
        delete simulator;
    }
}

