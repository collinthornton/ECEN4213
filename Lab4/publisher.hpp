#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <string>
#include <thread>
#include <mutex>

using namespace std;

class Publisher {
    public:
        Publisher(string serialPort, int baudRate);
        ~Publisher();
        void run();
        void stop();
        int speed; // mm/s
        int radius; // mm
        int kobukiId;
        void rotate(float theta, float omega);
        void moveDist(float distance, float velocity, float radius);
        void move(float velocity, float radius);
    private:
        void runP();
        int baud;
        string serialPort;
        thread runThread;
        bool stopThread;
        void sendMovement();
};


#endif //PUBLISHER_HPP