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

class Simulator
{
    public:
        Simulator();
        virtual ~Simulator();
        //
        void doSimulation();
        // 各イベントごとの処理
        void doContentRequest();
        void doReceivePacket();
        void doSendPacket();
    private:
        EvaluationManager* evaluationManager;
        RelationalGraph* relationalGraph;
        PhysicalNetwork* physicalNetwork;
        EventManager* eventManager;
        //

};

#endif /* defined(__SocialCast__Node__) */

