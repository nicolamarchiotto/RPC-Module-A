#ifndef GRAVITYCOULFRICT_CONTROLLER_H
#define GRAVITYCOULFRICT_CONTROLLER_H

#include "../lib/forecastnucleoframework/include/forecast/Controller.hpp"

namespace forecast
{
    class GravCoulombFrictCompController : public Controller
    {
    public:
        /**
         * @brief Construct a new controller object.
         **/
        GravCoulombFrictCompController();

        /**
         * @brief Construct a new controller object. This constructor initialize the controller.
         * @param p_kp
         * @param p_kd
         * @param p_ki
         **/
        GravCoulombFrictCompController(float p_kp, float p_kd, float p_ki);

        /**
         * @brief control initialization
         *
         * @param a vector containing the constants representing the parameters
         **/
        virtual bool init(const std::vector<float> &params) override;

        /**
         * @brief Get Return the names of the parameters.
         *
         * @return std::vector<std::string>
         */
        virtual std::vector<std::string> getParamNames() const override;

        /**
         * @brief Implements the control law
         */
        virtual float process(const IHardware *hw,
                              float ref,
                              float dref = 0,
                              float ddref = 0) override;

    protected:
        float p_kp = 0.0f;
        float p_kd = 0.0f;
        float p_ki = 0.0f;

        float mb = 0.0f;
        float f = 0.0f;

        float out;
    };
}
#endif