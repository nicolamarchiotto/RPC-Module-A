#ifndef DISCRETEPOS_CONTROLLER_H
#define DISCRETEPOS_CONTROLLER_H

#include "../lib/forecastnucleoframework/include/forecast/Controller.hpp"
#include "utility/filters/AnalogFilter.hpp"
#include "utility/filters/DigitalFilter.hpp"

namespace forecast
{
    class DiscretePosController : public Controller
    {
    public:
        /**
         * @brief Construct a new controller object.
         **/
        DiscretePosController();

        /**
         * @brief Construct a new controller object. This constructor initialize the controller.
         * @param p_kp
         * @param p_kd
         * @param p_ki
         **/
        DiscretePosController(float p_kp, float p_kd, float p_ki);

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

        float ref_k_m_1 = 0.0f;
        float theta_k_m_1 = 0.0f;
        float out_k_m_1 = 0.0f;

        float ref_k_m_2 = 0.0f;
        float theta_k_m_2 = 0.0f;
        float out_k_m_2 = 0.0f;

        utility::AnalogFilter *analFilter;
        utility::DigitalFilter *digitalFilter;
        
        bool once = false;
        bool twice = false;

        float out_k = 0.0f;
    };
}
#endif