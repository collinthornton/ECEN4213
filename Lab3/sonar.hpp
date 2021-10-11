#ifndef SONAR_H
#define SONAR_H

#include <thread>
#include <mutex>

class SONAR {
    public:
        SONAR(short signalPin);         // Initialize sonar class
        void stop(void);                // Stop thread
        void run(void);                 // Start thread
        double read(void);              // Read current estimate
        float smoothingFactor = .6;     // Adjust aggressiveness of filter.
        float prevDistance = 0;       
    private:
        void runP(void);                // Private run function (multithreaded)
        
        short signalPin;

        std::mutex m;                   // Mutex to protect shared data

        bool stopThread = false;
        std::thread runThread;          // Thread to continuously poll distance



        double output = -20;
        double tMax =  18.5e3;

};



#endif // SONAR_H