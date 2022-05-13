#ifndef FORECAST_APP_HPP
#define FORECAST_APP_HPP

#include <USART/USART_interface.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "Status.hpp"
#include "com.hpp"
namespace forecast {

class Controller;  // foreward initialization

class App {
    friend class IHardware;

   protected:
    USART_interface pc;
    const size_t txBuffSz;
    const size_t rxBuffSz;

    bool handshake;
    size_t cycleCount;
    Status status;

   public:
    App(unsigned int txBuffSz = 256,
        unsigned int rxBuffSz = 256,
        unsigned int baudrate = 921600);

    virtual ~App() = default;

    /**
     * @brief It logs an error through the serial
     *
     */
    void logError(const std::string& msg);

    /**
     * @brief Requires a simple float value for the user, if a name is passed
     * this will be showed to the user.
     *
     * @param valueName
     * @return float
     */
    float requireFloatValue(const std::string& valueName = "");

    /**
     * @brief Send a message to the pc.
     *
     * It is a time expensive function to not use for logging functionalities.
     *
     * @param msg Message to send.
     */
    void sendMessage(const std::string& msg);

    /**
     * @brief This method will wait for a message from the pc to be sure that
     * the connection is established
     *
     */
    bool waitConnection();

    /**
     * @brief Checks if the App is ready to run
     *
     * @return true if the object is fully initialized
     * @return false if the object is not fully initialized
     */
    virtual bool isOk();

    /**
     * @brief Execs the function execControl at the control frequency.
     *
     * @param[in] freq The frequency of the loop in Hz. Default to 1 KHz
     */
    bool execControlLoop(unsigned long freq = 1000);
    ///////////////////// Abstracts ////////////////////////////////////////////

    /**
     * @brief Execs one cicle of the control loop.
     *
     * This method should be called at a specified frequency to garantee to have
     * a good control.
     *
     */
    virtual bool execControl() = 0;

    /**
     * @brief It logs the values returned by the logger callback function
     *
     */
    virtual void logInfo() = 0;

   protected:
    void sendData(com::Type t, const void* data, size_t sz, bool wait = false);
    ssize_t receiveData(com::Type& t, void* buff, size_t maxSz = 0u);

    std::vector<float> requireControllerParams(
        const std::unique_ptr<Controller>& ctrl,
        const std::string& ctrlName);

    /**
     * @brief A fatal error occurred and there is no way to recover
     *
     * In a case like this the application will try to log on the logError and
     * then it will call std::abort().
     *
     * This kind of fatal error could occurre for example in case the app is
     * calling an hardware functionality that is not available.
     *
     * @param msg[in] the message to write on the serial
     */
    void fatalError(const std::string& msg);

    /**
     * @brief Returns the dt of the ticker that's maintaining the loop
     *
     * N.B. This function should not be called outside the execControl method
     *
     * @return uint32_t
     */
    uint32_t get_ticker_dt() const;

    /**
     * @brief Code that should run just before the begining of the control loop
     *
     * It could be use for inizializing some safety parameters inisde the hw or
     * for setting the correct time to some interface.
     *
     * The default implementation is empty.
     */
    virtual inline void preLoopInit(){};

    /**
     * @brief Code that should be always run at the end of the main loop
     *
     * This code will be run at the end of the main loop, doesn't matter if
     * the end is a failure or a success.
     *
     * This method should be override to for example disable the motor[s].
     *
     */
    virtual inline void loopFinalize(){};
};
}  // namespace forecast

#endif  // FORECAST_APP_HPP