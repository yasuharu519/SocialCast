#include "ContentPacket.hpp"

// Content class
Content::Content(int _contentID)
{
    contentID = _contentID;
}

Content::~Content()
{
}

vector<Packet*> Content::createPacket()
{
    vector<Packet*> output;
    for(int i = 0; i < PACKET_NUM; ++i)
    {
        output.push_back(new Packet(this));
    }
    return output;
}


// Packet class
int Packet::newestPacketID = 0;

Packet::Packet(Content* _content)
{
    content = _content;
}

Packet::~Packet()
{
}


