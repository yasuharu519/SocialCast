#ifndef __EVALUATION_MANAGER_HPP__
#define __EVALUATION_MANAGER_HPP__

using namespace std;


class EvaluationManager{
    public:
        // Constructor/Destructor
        EvaluationManager();
        virtual ~EvaluationManager();
        void clearEvaluationData();
        // PacketInQueue
        void addTimePacketInQueue(double _inQueueTime);
        double getAverageTimePacketInQueue();
        // ContentGetTime
        void addTimeContentGet(double _contentGetTime);
        double getAverageTimeContentGet();
    private:
        long trialTimes;
        long numPathHopCount;
        long numPackets;
        long numContents;
        double distributeLatency;
        double penalty;
        double timePacketInQueue;
        double timeContentGet;
};

#endif
