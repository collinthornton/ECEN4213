
#ifndef PID_H
#define PID_H

class PID {
    public:
        double Kp, Ki, Kd;
        double P,I,D;

        PID(double Kp, double Ki, double Kd);
        int compute(double input, double setpoint);
        void reset();

    private:
        double integral, prevError, prevTime;
};

#endif //PID