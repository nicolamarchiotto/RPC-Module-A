#ifdef TARGET_STM32L4

#include <forecast/config/analog_torque_sensor.h>
#include <forecast/platforms/RPC/RPCHardware.hpp>

#include <forecast/config/escon_motor.h>
#include <forecast/config/AB_encoder_motor.h>
#include "utility/filters/AnalogFilter.hpp"

using namespace forecast;

RPCHardware::RPCHardware(App &app)
    : IHardware(app),
      strain_gauge(TORQUE_SENSOR_PIN,
                   ADC_PCLK2,
                   ADC_Right,
                   ADC_12s5,
                   ADC_12b,
                   ADC_Continuous,
                   ADC_Dma,
                   TORQUE_SENSOR_BUFFER_SIZE),
      encoder(motorEncoder::CPR, motorEncoder::GEAR_RATIO)
{
    // 
    // double b[5] = {28.03, 34020, 460500, 777700, 0};       // NUM
    // double a[5] = {1, 2402, 1500000, 69970000, 777700000}; // DEN
    // analFilter = new utility::AnalogFilter(4, a, b);
}

bool RPCHardware::motorControlInit()
{
    MotorConfiguration conf;
    conf.enable = motorControl::MOTOR_ENABLE_PIN;
    conf.currFeedback = motorControl::MOTOR_CURRENT_FEEDBACK_PIN;
    conf.analog = motorControl::MOTOR_ANALOG_PIN;

    /* Control motor */
    motor.reset(new EsconMotor(conf, motorControl::KT, motorControl::JM,
                               motorControl::MAX_CURR));

    return static_cast<bool>(motor);
}

Status RPCHardware::init()
{
    if (strain_gauge.enable() == -1)
    {
        return Status::TORQUE_SENSOR_INIT_ERR;
    }

    if (not motorControlInit())
    {
        return Status::CONTROL_MOTOR_INIT_ERR;
    }

    if (not encoder.setLPTIM1())
        return Status::MOTOR_ENCODER_INIT_ERR;

    if (not encoder.setVelocity())
        return Status::MOTOR_ENCODER_INIT_ERR;

    // waiting for reading the offset.
    wait(3);

    // reading and storing the offset
    strain_offset = strain_gauge.read_average_float() * 3.3f;

    return Status::NO_ERROR;
}

void RPCHardware::update(float dt)
{

    static float t_offset = us_ticker_read() / 1e6;

    /* Time update */
    this->dt = dt;
    t = us_ticker_read() / 1e6 - t_offset;

    /* Motor encoder is mounted in the opposite position wrt env encoder */
    thetaM = -encoder.getAngleRad();       // notice the minus sign due to laziness
    dthetaM = (thetaM - prev_thetaM) / dt; //  encoder.getVelocityRad(dt); //
    ddthetaM = (dthetaM - prev_dthetaM) / dt;
    prev_thetaM = thetaM;
    prev_dthetaM = dthetaM;

    /* Control motor update  (from Escon feedback) */
    tauM = motor->getTorqueFeedback();
    dtauM = (tauM - prev_tauM) / dt;
    ddtauM = (dtauM - prev_dtauM) / dt;
    prev_tauM = tauM;
    prev_dtauM = dtauM;

    /* Control motor update  (from the stain gauge) */
    tauS = -(strain_gauge.read_average_float() * 3.3f - strain_offset) *
           voltage_to_kg * 9.81f * length; // Nm
    dtauS = (tauS - prev_tauS) / dt;
    ddtauS = (dtauS - prev_dtauS) / dt;
    prev_tauS = tauS;
    prev_dtauS = dtauS;
}

// float RPCHardware::getMag(float f) const{
//     return analFilter->getMagnitudeHz(f);
// }

#endif // TARGET_STM32L4