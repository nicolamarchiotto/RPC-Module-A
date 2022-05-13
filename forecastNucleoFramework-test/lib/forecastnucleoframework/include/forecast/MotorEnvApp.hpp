#ifndef FORECAST_MOTOR_ENV_APP_HPP
#define FORECAST_MOTOR_ENV_APP_HPP

#ifndef TARGET_STM32F4
#error "The Class MotorEnvApp is available only for the platform TARGET_STM32F4"
#else

#include <USART/USART_interface.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "AbstractApp.hpp"

#include "Controller.hpp"
#include "Hardware.hpp"
#include "Status.hpp"
#include "com.hpp"

namespace forecast {

using LoggerCallback = std::function<std::vector<
    float>(float, float, const Hardware*, const Controller*, Controller*)>;
using RefGen = std::function<float(const Hardware*)>;

class MotorEnvApp : public App {
   private:
    // need of an instance of forecastHW created by the app
    Hardware hw;
    std::unique_ptr<Controller> environment;
    std::unique_ptr<Controller> motor;

    LoggerCallback logger;

    RefGen envRefGen;
    RefGen motorRefGen;

   public:
    MotorEnvApp(unsigned int txBuffSz = 256,
                unsigned int rxBuffSz = 256,
                unsigned int baudrate = 921600);

    void setEnvironment(std::unique_ptr<Controller> env);
    void setMotor(std::unique_ptr<Controller> motor);
    void setEnvironment(Controller* env);
    void setMotor(Controller* motor);
    void setLogger(LoggerCallback logger);
    void setEnvRefGen(RefGen gen);
    void setMotorRefGen(RefGen gen);
    void setMotorRefSignal(float amplitude_step_1 = NAN,
                           float amplitude_step_2 = NAN,
                           float amplitude_step_3 = NAN,
                           float amplitude_step_4 = NAN,
                           float step_duration = NAN,
                           float sweep_amplitude = NAN,
                           float sweep_duration = NAN,
                           float sweep_max_freq = NAN);
    void setEnvRefSweep(float amplitude = NAN,
                        float frequency_start = NAN,
                        float frequency_end = NAN,
                        float duration = NAN);

    /**
     * @brief Checks if the App is ready to run
     *
     * @return true if the object is ready to exec the control
     * @return false if the object is not ready to exec the control loop
     */
    virtual bool isOk() override;

    /**
     * @brief Execs one cicle of the control loop.
     *
     * This method should be called at a specified frequency to garantee to have
     * a good control.
     *
     */
    virtual bool execControl() override;

    /**
     * @brief It logs the values returned by the logger callback function
     *
     */
    virtual void logInfo() override;

    virtual void preLoopInit() override;

    virtual void loopFinalize() override;

    //==========================communication===================================

    /**
     * @brief Requires the parameters for the motor controller from the pc
     * interface
     *
     */
    bool requireMotorParams();

    /**
     * @brief Requires the parameters for the enviorment controller from the pc
     * interface
     *
     */
    bool requireEnvironmentParams();
};

}  // namespace forecast

#endif // TARGET_STM32F4
#endif // FORECAST_MOTOR_ENV_APP_HPP