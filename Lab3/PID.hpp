
#ifndef PID_H
#define PID_H

class PID {
    public:
        double Kp, Ki, Kd;  // Controller gains
        double P,I,D;       // Controller output components

        PID(double Kp, double Ki, double Kd, double minPeriod);
        int compute(double input, double setpoint);     // Compute the PID output
        void reset();                                   // Reset the integral error

    private:
        double integral, prevError, prevTime;
        double minPeriod;
};

#endif //PID