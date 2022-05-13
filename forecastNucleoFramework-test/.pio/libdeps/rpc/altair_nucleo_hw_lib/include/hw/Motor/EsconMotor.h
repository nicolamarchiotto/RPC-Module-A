#ifndef ESCON_MOTOR_H
#define ESCON_MOTOR_H

#include <mbed.h>
#include <hw/Periphericals/AnalogInput.h>

struct MotorConfiguration
{
    // PinName pwm;
    PinName enable;
    PinName currFeedback;
    PinName analog;
};

class EsconMotor {
public:
    EsconMotor(MotorConfiguration conf, float Kt, float Jm, float maxCurr);
    
    void setEnable(bool enable);
    bool getEnable();
    // void setPwmPeriod(float pwm_period);

    void setCurrent(float curr);
    void setTorque(float tau_m);

    float getCurrentFeedback();
    float getTorqueFeedback();

    inline float getKT() { return Kt; }
    inline float getJm() { return Jm; }
    inline float getMaxCurr() { return maxCurr; }

protected:

    float Kt;
    float Jm;
    float maxCurr;

private:

    // PwmOut pwmPhase;
    DigitalOut enable;
    AnalogInput currFeedback;
    AnalogOut analogPhase;
};

#endif
