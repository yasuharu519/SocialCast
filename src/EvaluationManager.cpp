#include "EvaluationManager.hpp"

// Constructor/Destructor
EvaluationManager::EvaluationManager()
{
    clearEvaluationData();
}

EvaluationManager::~EvaluationManager()
{
    // 後始末
}

//Public
void EvaluationManager::clearEvaluationData()/*{{{*/
{
    numPathHopCount = 0;
    trialTimes = 0;
    numPackets = 0;
    distributeLatency = 0.0;
    penalty = 0.0;
    timePacketInQueue = 0.0;
    timeContentGet = 0.0;
}/*}}}*/

void EvaluationManager::addTimePacketInQueue(double _inQueueTime)/*{{{*/
{
    ++numPackets;
    timePacketInQueue += _inQueueTime;
}/*}}}*/

double EvaluationManager::getAverageTimePacketInQueue()/*{{{*/
{
    return timePacketInQueue / numPackets;
}/*}}}*/

void EvaluationManager::addTimeContentGet(double _contentGetTime)/*{{{*/
{
    ++numContents;
    timeContentGet += _contentGetTime;
}/*}}}*/

double EvaluationManager::getAverageTimeContentGet()/*{{{*/
{
    return timeContentGet / numContents;
}/*}}}*/
