#ifndef FORECAST_IHARDWARE_HPP
#define FORECAST_IHARDWARE_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "AbstractApp.hpp"

namespace forecast {
class IHardware {
   protected:
    App& app;

   public:
    /**
     * @brief   Hardware constructor
     */
    IHardware(App& app) : app(app){};

    /**
     * @brief   Return the hw time t of the last update.
     *
     * @return  t
     */
    virtual float getT() const {
        app.fatalError("Hardware getT() function not implemented");
    }
    /**
     * @brief   Return the hw dt used in the last update.
     *
     * @return  dt
     */
    virtual float getDT() const {
        app.fatalError("Hardware getDT() function not implemented");
    }

    /**
     * @brief   Set the start time of the experiment.
     *
     * @param  time The start time of the experiment
     */
    virtual inline void setStartT(float time) {
        app.fatalError("Hardware setStartT() function not implemented");
    }

    /**
     * @brief   Return the start time of the experiment.
     *
     * @return  start_t
     */
    virtual inline float getStartT() const {
        app.fatalError("Hardware getStartT() function not implemented");
    }

    /**
     * @brief   Return the hw time t from the start of the experiment.
     *
     * @return  curr_t
     */
    virtual inline float getCurrT() const {
        app.fatalError("Hardware getStartT() function not implemented");
    }

    /**std::make_unique<control::Control>()
     * @brief   Return the torque applied by the motor (current feedback)
     *
     * @return  motorTorqueFeedback
     */
    virtual inline float getTauM() const {
        app.fatalError("Hardware getTauM() function not implemented");
    }
    virtual inline float getDTauM() const {
        app.fatalError("Hardware getDTauM() function not implemented");
    }
    virtual inline float getDDTauM() const {
        app.fatalError("Hardware getDDTauM() function not implemented");
    }

    /**
     * @brief   Return the torque applied by the environment (current feedback)
     *
     * @return  tau_e
     */
    virtual inline float getTauE() const {
        app.fatalError("Hardware getTauE() function not implemented");
    }
    virtual inline float getDTauE() const {
        app.fatalError("Hardware getDTauE() function not implemented");
    }
    virtual inline float getDDTauE() const {
        app.fatalError("Hardware getDDTauE() function not implemented");
    }

    /**
     * @brief   Return the torque measured by the spring
     *
     * @return  tau_s
     */
    virtual inline float getTauS() const {
        app.fatalError("Hardware getTauS() function not implemented");
    }
    virtual inline float getDTauS() const {
        app.fatalError("Hardware getDTauS() function not implemented");
    }
    virtual inline float getDDTauS() const {
        app.fatalError("Hardware getDDTauS() function not implemented");
    }

    /**
     * @brief   Return the torque measured by the torque sensor
     *
     * @return  tau_sensor
     */
    virtual inline float getTauSensor() const {
        app.fatalError("Hardware getTauSensor() function not implemented");
    }
    virtual inline float getDTauSensor() const {
        app.fatalError("Hardware getDTauSensor() function not implemented");
    }
    virtual inline float getDDTauSensor() const {
        app.fatalError("Hardware getDDTauSensor() function not implemented");
    }

    /**
     * @brief   Return the angle radius measured by the encoder of the motor.
     *
     * @return  thetaM
     */
    virtual inline float getThetaM() const {
        app.fatalError("Hardware getThetaM() function not implemented");
    }
    virtual inline float getDThetaM() const {
        app.fatalError("Hardware getDThetaM() function not implemented");
    }
    virtual inline float getDDThetaM() const {
        app.fatalError("Hardware getDDThetaM() function not implemented");
    }

    /**
     * @brief   Return the angle radius measured by the encoder of the
     * environment.
     *
     * @return  thetaE
     */
    virtual inline float getThetaE() const {
        app.fatalError("Hardware getThetaE() function not implemented");
    }
    virtual inline float getDThetaE() const {
        app.fatalError("Hardware getDThetaE() function not implemented");
    }
    virtual inline float getDDThetaE() const {
        app.fatalError("Hardware getDDThetaE() function not implemented");
    }

    /**
     * @brief   Return the angle radius measured by the encoder of the
     * motor environment.
     *
     * @return  thetaEnvMotor
     */
    virtual inline float getThetaEnvMotor() const {
        app.fatalError("Hardware getThetaEnvMotor() function not implemented");
    }
    virtual inline float getDThetaEnvMotor() const {
        app.fatalError("Hardware getDThetaEnvMotor() function not implemented");
    }
    virtual inline float getDDThetaEnvMotor() const {
        app.fatalError(
            "Hardware getDDThetaEnvMotor() function not implemented");
    }

    virtual inline float getOutput() const {
        app.fatalError("Hardware getOutput() function not implemented");
    }
    virtual inline float getDOutput() const {
        app.fatalError("Hardware getDOutput() function not implemented");
    }
    virtual inline float getDDOutput() const {
        app.fatalError("Hardware getDDOutput() function not implemented");
    }

    virtual inline void enableControlMotor() {
        app.fatalError(
            "Hardware enableControlMotor() function not implemented");
    }

    virtual inline void enableEnvMotor() {
        app.fatalError("Hardware enableEnvMotor() function not implemented");
    }

    virtual inline void disableControlMotor() {
        app.fatalError(
            "Hardware disableControlMotor() function not implemented");
    }

    virtual inline void disableEnvMotor() {
        app.fatalError("Hardware disableEnvMotor() function not implemented");
    }

    virtual inline void setControlTorque(float torque) {
        app.fatalError("Hardware setControlTorque() function not implemented");
    }

    virtual inline void setEnvTorque(float torque) {
        app.fatalError("Hardware setEnvTorque() function not implemented");
    }

    virtual void update(float dt) = 0;
};
}  // namespace forecast

#pragma GCC diagnostic pop

#endif // FORECAST_IHARDWARE_HPP