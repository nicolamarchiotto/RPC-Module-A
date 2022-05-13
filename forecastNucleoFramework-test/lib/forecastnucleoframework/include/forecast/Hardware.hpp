#ifndef FORECAST_HARDWARE_HPP
#define FORECAST_HARDWARE_HPP

#ifndef TARGET_STM32F4
#error "The Class Hardware is available only for the platform TARGET_STM32F4"
#else


#include <hw/DigitalEncoder/DigitalEncoderAB.h>
#include <hw/Motor/EsconMotor.h>
#include <mbed.h>

#include <utility/math.hpp>

#include "Status.hpp"

#include "IHardware.hpp"

namespace forecast {
class Hardware : public IHardware {
   public:
    /**
     * @brief   Hardware constructor
     */
    Hardware(App& app) : IHardware(app) {}

    // ~Hardware() {
    //     delete encoder;
    //     delete motor;
    //     delete strain_gauge;
    // }

    /**
     * @brief   Initialization of the Hardware
     */
    Status init();

    /**
     * @brief   Return the hw time t of the last update.
     *
     * @return  t
     */
    virtual inline float getT() const override { return t; }
    /**
     * @brief   Return the hw dt used in the last update.
     *
     * @return  dt
     */
    virtual inline float getDT() const override { return dt; }

    /**
     * @brief   Set the start time of the experiment.
     *
     * @param  time The start time of the experiment
     */
    inline void setStartT(float time) override { start_t = time; }

    /**
     * @brief   Return the start time of the experiment.
     *
     * @return  start_t
     */
    virtual inline float getStartT() const override { return start_t; }

    /**
     * @brief   Return the hw time t from the start of the experiment.
     *
     * @return  curr_t
     */
    virtual inline float getCurrT() const override { return t - start_t; }

    /**std::make_unique<control::Control>()
     * @brief   Return the torque applied by the motor (current feedback)
     *
     * @return  motorTorqueFeedback
     */
    virtual inline float getTauM() const override { return tauM; }
    virtual inline float getDTauM() const override { return dtauM; }
    virtual inline float getDDTauM() const override { return ddtauM; }

    /**
     * @brief   Return the torque applied by the environment (current feedback)
     *
     * @return  tau_e
     */
    virtual inline float getTauE() const override { return tauE; }
    virtual inline float getDTauE() const override { return dtauE; }
    virtual inline float getDDTauE() const override { return ddtauE; }

    /**
     * @brief   Return the torque measured by the spring
     *
     * @return  tau_s
     */
    virtual inline float getTauS() const override { return tauS; }
    virtual inline float getDTauS() const override { return dtauS; }
    virtual inline float getDDTauS() const override { return ddtauS; }

    /**
     * @brief   Return the torque measured by the torque sensor
     *
     * @return  tau_sensor
     */
    virtual inline float getTauSensor() const override { return tauSensor; }
    virtual inline float getDTauSensor() const override { return dtauSensor; }
    virtual inline float getDDTauSensor() const override { return ddtauSensor; }

    /**
     * @brief   Return the angle radius measured by the encoder of the motor.
     *
     * @return  thetaM
     */
    virtual inline float getThetaM() const override { return thetaM; }
    virtual inline float getDThetaM() const override { return dthetaM; }
    virtual inline float getDDThetaM() const override { return ddthetaM; }

    /**
     * @brief   Return the angle radius measured by the encoder of the
     * environment.
     *
     * @return  thetaE
     */
    virtual inline float getThetaE() const override { return thetaE; }
    virtual inline float getDThetaE() const override { return dthetaE; }
    virtual inline float getDDThetaE() const override { return ddthetaE; }

    /**
     * @brief   Return the angle radius measured by the encoder of the
     * motor environment.
     *
     * @return  thetaEnvMotor
     */
    virtual inline float getThetaEnvMotor() const override {
        return thetaEnvMotor;
    }
    virtual inline float getDThetaEnvMotor() const override {
        return dthetaEnvMotor;
    }
    virtual inline float getDDThetaEnvMotor() const override {
        return ddthetaEnvMotor;
    }

    virtual inline float getOutput() const override { return output; }
    virtual inline float getDOutput() const override { return doutput; }
    virtual inline float getDDOutput() const override { return ddoutput; }

    /**
     * @brief   Update the Hardware by reading the value from the physical hw
     *
     * @param   Torque that has to be given to the motor for actuating the
     * control Torque that has to be given to the environment for the simulation
     *
     * @param   dt is the delta time in seconds to use for the
     * calculations for controls
     */
    // void update(float controlTorque, float envTorque, float dt);
    virtual void update(float dt) override;

    inline void enableControlMotor() override {
        control_motor->setEnable(true);
    }

    inline void enableEnvMotor() override { env_motor->setEnable(true); }

    inline void disableControlMotor() override {
        control_motor->setEnable(false);
    }

    inline void disableEnvMotor() override { env_motor->setEnable(false); }

    // inline void resetEncoder() { encoder_offset = encoder->getAngleRad(); }
    inline void setControlTorque(float torque) override {
        control_motor->setTorque(torque);
    }

    inline void setEnvTorque(float torque) override {
        env_motor->setTorque(torque);
    }

   protected:
    bool motorEncoderInit();  /// < Initialize the motor encoder

    bool envEncoderInit();  /// < Initialize the environment encoder

    bool envMotorEncoderInit();  /// < Initialize the environment encoder

    bool motorControlInit();  ///< Initialize the motor

    bool motorEnvironmentInit();  ///< Initialize the motor

    bool torqueSensorInit();  ///< Initialize the torque sensor

    DigitalEncoderAB* encoder_motor = nullptr;  ///< Motor encoder
    DigitalEncoderAB* encoder_env = nullptr;    ///< Environment encoder
    DigitalEncoderAB* encoder_env_motor =
        nullptr;  ///< Environment motor encoder

    EsconMotor* control_motor = nullptr;  ///< Motor used for the control
    EsconMotor* env_motor = nullptr;  ///< Motor used for the env. simulation

    AnalogInput* torque_sensor = nullptr;  ///< Torque sensor

    float t, dt;
    float start_t;

    float tauM;
    float dtauM;
    float ddtauM;

    float tauE;
    float dtauE;
    float ddtauE;

    float tauS;
    float dtauS;
    float ddtauS;

    float tauSensor;
    float dtauSensor;
    float ddtauSensor;

    float thetaM;
    float dthetaM;
    float ddthetaM;

    float thetaE;
    float dthetaE;
    float ddthetaE;

    float thetaEnvMotor;
    float dthetaEnvMotor;
    float ddthetaEnvMotor;

    float prev_tauM;
    float prev_dtauM;

    float prev_tauE;
    float prev_dtauE;

    float prev_tauS;
    float prev_dtauS;

    float prev_tauSensor;
    float prev_dtauSensor;

    float prev_thetaM;
    float prev_dthetaM;

    float prev_thetaE;
    float prev_dthetaE;

    float prev_thetaEnvMotor;
    float prev_dthetaEnvMotor;

    float output;
    float doutput;
    float ddoutput;
};
}  // namespace forecast

#endif // TARGET_STM32F4
#endif  // FORECAST_HARDWARE_HPP