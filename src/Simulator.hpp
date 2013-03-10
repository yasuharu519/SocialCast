//
//  Simulator.h
//  SocialCast
//
//  Created by 澤田 泰治 on 2012/12/03.
//  Copyright (c) 2012年 澤田 泰治. All rights reserved.
//

#ifndef __SocialCast__Simulator__
#define __SocialCast__Simulator__

#include "PhysicalNetwork.hpp"
#include "RelationalGraph.hpp"
#include "EvaluationManager.hpp"
#include "EventManager.hpp"

#define LAMBDA 0.8
// 100Mbps
#define BANDWIDTH 100000000.0
// 10MB = 80Mbits
#define CONTENT_SIZE (10000000.0 * 8) 
// MTU 1500bytes
#define PACKET_SIZE (1500.0 * 8)
// 100 micro second
#define PACKET_KANKAKU ((PACKET_SIZE / BANDWIDTH) + 0.0001)
// packet num
#define PACKET_NUM ((int)CONTENT_SIZE / PACKET_SIZE)
class Simulator
{
    public:
        Simulator();
        virtual ~Simulator();
        //
        void doSimulation();
        // 次のリクエストイベントの作成
        void createNextRequestEvent(double _time);
        // 各イベントごとの処理
        void doContentRequest(double time);
        void doReceivePacket(ReceivePacketEvent* event);
        void doSendPacket(SendPacketEvent* event);
    private:
        EvaluationManager* evaluationManager;
        RelationalGraph* relationalGraph;
        PhysicalNetwork* physicalNetwork;
        EventManager* eventManager;
        bool useProposedMethod;
        //
        void generateSendPacketEventFromTime(double _time, int _packetID);

};

#endif /* defined(__SocialCast__Node__) */

