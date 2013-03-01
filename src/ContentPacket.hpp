//
//  Simulator.h
//  SocialCast
//
//  Created by 澤田 泰治 on 2012/12/03.
//  Copyright (c) 2012年 澤田 泰治. All rights reserved.
//

#ifndef __CONTENT_PACKET__
#define __CONTENT_PACKET__
#include <vector>
#include "Simulator.hpp"

class Content
{
    public:
        Content(int _contentID);
        virtual ~Content();
        vector<Packet*> createPacket();
    private:
        int contentID;
};

class Packet
{
    public:
        Packet(Content* _content);
        virtual ~Packet();
    private:
        static int newestPacketID;
        int packetID; // リクエストされた際に決めるルートのkeyとなる
        Content* content;
};

#endif 


