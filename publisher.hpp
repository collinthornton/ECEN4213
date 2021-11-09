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

        enum Bumper {
            BUMP_RIGHT = 1,
            BUMP_CENTER = 2,
            BUMP_LEFT = 4
        };
        enum WheelDrop {
            WHEEL_RIGHT = 1,
            WHEEL_LEFT = 2
        };
        enum Cliff {
            CLIFF_RIGHT = 1,
            CLIFF_CENTER = 2,
            CLIFF_LEFT = 4
        };
        enum Button {
            BTN0 = 1,
            BTN1 = 2,
            BTN2 = 4
        };

        Bumper bumperSensor;
        WheelDrop wheelSensor;
        Cliff cliffSensor;
        Button button;


    private:
        void runP();
        int baud;
        string serialPort;
        thread runThread;
        bool stopThread;
        void sendMovement();
        void readSensors();
};


#endif //PUBLISHER_HPP