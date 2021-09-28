#ifndef SONAR_H
#define SONAR_H

#include <thread>
#include <mutex>

class SONAR {
    public:
        SONAR(short signalPin);
        void stop(void);
        void run(void);
        double read(void);
        float smoothingFactor = 0.05;
        float prevDistance = 0;
    private:
        void runP(void);
        
        short signalPin;

        std::mutex m;

        bool stopThread = false;
        std::thread runThread;



        double output = 0;
        double tMax =  18.5e-3;

};



#endif // SONAR_H