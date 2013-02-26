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

class Simulator
{
    public:
        Simulator();
        virtual ~Simulator();
        //
        void doSimulation();
    private:
        EvaluationManager* evaluationManager;
        RelationalGraph* relationalGraph;
        PhysicalNetwork* physicalNetwork;
        VertexList physicalNodeIDList;
        //
        Vertex chooseContentRequestedUser();

};

#endif /* defined(__SocialCast__Node__) */

