#ifndef SWIP_CONTROLLER_H
#define SWIP_CONTROLLER_H

#include "../lib/forecastnucleoframework/include/forecast/Controller.hpp"
#include "utility/filters/AnalogFilter.hpp"
#include "utility/filters/DigitalFilter.hpp"

namespace forecast
{
    class SwipController : public Controller
    {
    public:
        /**
         * @brief Construct a new controller object.
         **/
        SwipController();

        /**
         * @brief Construct a new controller object. This constructor initialize the controller.
         * @param p_kp
         * @param p_kd
         * @param p_ki
         **/
        SwipController(float p_kp, float p_kd, float p_ki);

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

        utility::AnalogFilter *analFilter;
        utility::AnalogFilter *analFilterCtrl;
        float t = 0.0f;
        float kt = 0.0f;
        float max_current = 0.0f;
        float u_sat = 0.0f;
        float scal_factor = 0.0;
        float alpha = 0.0;
    public:
        float ref_swip;
    };
}
#endif