#ifndef FORECAST_RPC_APP_HPP
#define FORECAST_RPC_APP_HPP

#ifndef TARGET_STM32L4
#error "The Class RPCHardware is available only for the platform TARGET_STM32L4"
#else

#include <USART/USART_interface.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "../../AbstractApp.hpp"

#include "../../Controller.hpp"
#include "RPCHardware.hpp"

namespace forecast {

using LoggerCallback = std::function<
    std::vector<float>(float, const RPCHardware*, const Controller*)>;
using RefGen = std::function<float(const RPCHardware*)>;

class RPCApp : public App {
   private:
    // need of an instance of forecastHW created by the app
    RPCHardware hw;
    std::unique_ptr<Controller> motor;

    LoggerCallback logger;

    RefGen motorRefGen;

   public:
    RPCApp(unsigned int txBuffSz = 256,
           unsigned int rxBuffSz = 256,
           unsigned int baudrate = 921600);

    void setMotor(std::unique_ptr<Controller> motor);
    void setMotor(Controller* motor);
    void setLogger(LoggerCallback logger);
    void setMotorRefGen(RefGen gen);

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
};

}  // namespace forecast

#endif  // TARGET_STM32L4
#endif  // FORECAST_RPC_APP_HPP