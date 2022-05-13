#include "hw/Motor/EsconMotor.h"

EsconMotor::EsconMotor(MotorConfiguration conf, float Kt, float Jm, float maxCurr) : 
    // pwmPhase(conf.pwm),
    enable(conf.enable), 
    currFeedback(conf.currFeedback),
    analogPhase(conf.analog),
    Jm(Jm),
    Kt(Kt),
    maxCurr(maxCurr)
{
    //ntd
}

void EsconMotor::setEnable(bool enable)
{
    this->enable = (enable == true) ? 1 : 0;
}

bool EsconMotor::getEnable()
{
    return (enable == 1) ? true : false;
}

// void EsconMotor::setPwmPeriod(float pwm_period)
// {
//     pwmPhase.period(pwm_period);
// }

void EsconMotor::setCurrent(float curr)
{
    float value = (curr / maxCurr) * 0.5f + 0.5f;

    // saturation
    if (value < 0.0f) value = 0.0f;
    else if (value > 1.0f) value = 1.0f;

    analogPhase.write(value);
}

void EsconMotor::setTorque(float tau_m)
{
    setCurrent( tau_m / Kt );
}

float EsconMotor::getCurrentFeedback() {
    
    return ((currFeedback.read_last_float() - 0.5f) / 0.5f) * maxCurr;
    // return 1.0f;
}

float EsconMotor::getTorqueFeedback() {
    
    return getCurrentFeedback() * Kt;
}