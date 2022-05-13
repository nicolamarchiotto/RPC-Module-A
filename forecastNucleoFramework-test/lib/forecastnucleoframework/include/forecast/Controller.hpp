#ifndef FORECAST_CONTROLLER_HPP
#define FORECAST_CONTROLLER_HPP

#include <string>
#include <vector>

#include "IHardware.hpp"

namespace forecast {
class Controller {
   public:
    /**
     * @brief   Controller constructor
     *
     * @param   Number of parameters required for the initialization
     */
    Controller(size_t numberOfParams = 0);

    /**
     * @brief Default virtual destructor
     *
     */
    virtual ~Controller() = default;

    /**
     * @brief   Controller initialization. The implementation should take care
     * of setting to true the value of the initialized field of the controller
     *
     * @param   A vector containing the float params.
     */
    virtual bool init(const std::vector<float>& params) = 0;

    /**
     * @brief Get the names of the parameter in order. If not implemented it
     * will return a vector of strings: "param_1", ..., "param_N".
     *
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getParamNames() const;

    /**
     * @brief Get the number of parameters required for the initialization.
     *
     * @return size_t
     */
    virtual size_t getNumberOfParams() const; // as return {"KP","KI","KD"};
    // std::vector<std::string

    /**
     * @brief return if the controller is been initialized
     *
     * @return true
     * @return false
     */
    virtual bool isInitialized() const;

    /**
     * @brief   Implementation of the control law.
     *          The method produces an output given a reference (input).
     *
     * @param   Control reference
     */
    virtual float process(const IHardware* hw,
                          float ref,
                          float dref = 0,
                          float ddref = 0) = 0;

   protected:
    const size_t numberOfParams;
    bool initialized = false;
};
}  // namespace forecast

#endif