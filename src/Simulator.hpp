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
#include "Const.hpp"

class Simulator
{
    public:
        Simulator();
        virtual ~Simulator();
        //
        void doSimulation(int contentRequestTime, int contentCacheSize, bool useProposedMethod);
        // 次のリクエストイベントの作成
        void createNextRequestEvent(double _time);
        // 各イベントごとの処理
        void doContentRequest(double time, int contentCacheSize, bool useProposedMethod);
        void doReceivePacket(ReceivePacketEvent* event);
        //void doSendPacket(SendPacketEvent* event);
        void doContentSending(ContentStartSendingEvent* event);
        void doContentReceived(ContentReceivedEvent* event);
    private:
        EvaluationManager* evaluationManager;
        RelationalGraph* relationalGraph;
        PhysicalNetwork* physicalNetwork;
        EventManager* eventManager;
        bool useProposedMethod;
        bool searchFromRequestedUser;
        //
        void generateSendPacketEventFromTime(double _time, int _packetID);

};

#endif /* defined(__SocialCast__Node__) */

