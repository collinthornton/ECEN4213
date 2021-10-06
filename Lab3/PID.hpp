
#ifndef PID_H
#define PID_H

class PID {
    public:
        double Kp, Ki, Kd;
        double P,I,D;

        PID(double Kp, double Ki, double Kd, double minPeriod);
        int compute(double input, double setpoint);
        void reset();

    private:
        double integral, prevError, prevTime;
        double minPeriod;
};

#endif //PID