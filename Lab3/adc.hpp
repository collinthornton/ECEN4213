// Start ADC
// Running average of ADC (in separate thread)
// Provide read interface
#ifndef ADC_H
#define ADC_H

#include <thread>
#include <mutex>

class ADC {
    public:
        ADC(int i2cDeviceID);
        double readmv();        // Output reading in mV
        void run();             // Start thread
        void stop();            // Stop thread
        int adc;

    private:
        void runP();            // Reads ADC constantly (main part of thread)
        int readBits();         // Reads ADC bit values for later conversion

        int deviceID;
        int bitVal;
        int prevBitVal;

        double smoothingFactor = 1.0;
        
        std::mutex m;
        bool stopThread = false;
        std::thread runThread;

        double mvConversionFactor = 6144 / 2047;


};

#endif //ADC_H