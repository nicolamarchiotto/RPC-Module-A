#include "../include/DiscretePosController.hpp"
#include "utility/math.hpp"

using namespace forecast;

DiscretePosController::DiscretePosController() : Controller(3)
{
}

DiscretePosController::DiscretePosController(float p_kp, float p_kd, float p_ki) : Controller(3), p_kp(p_kp), p_kd(p_kd), p_ki(p_ki)
{
    Controller::initialized = true;
}

bool DiscretePosController::init(const std::vector<float> &params)
{
    if (params.size() != numberOfParams)
        return false;

    p_kp = params[0]; // position proportional
    p_kd = params[1]; // position derivative
    p_ki = params[2]; // position integrative

    out_k = 0.0;
    out_k_m_1 = 0.0f;
    
    once = false;
    twice = false;

    // PD POS
    //  double b[2] = {28.029, 28.029*11.7};
    //  double a[2] = {1, 1200};
    //  analFilter = new utility::AnalogFilter(1, a, b);

    // PID POS
    double b[3] = {24.64, 253.4, 646.4}; // NUM
    double a[3] = {1, 800, 0};           // DEN

    analFilter = new utility::AnalogFilter(2, a, b);


    double b_z[3] = {20.58, -41.06, 20.48}; // NUM
    double a_z[3] = {1, -1.667, 0.6667};           // DEN

    digitalFilter = new utility::DigitalFilter(2, a_z, b_z);

    return initialized = true;
}

float DiscretePosController::process(const IHardware *hw,
                                     float ref,
                                     float dref,
                                     float ddref)
{
    // DISCRETE PD POSITION CONTROL

    // if (once)
    // {
    //     out_k = 21.62 * (ref - hw->getThetaM()) - 21.5 * (ref_k_m_1 - theta_k_m_1) + 0.5385 * out_k_m_1;
    // }
    // else
    // {
    //     once = true;
    //     out_k = 21.62 * (ref - hw->getThetaM());
    // }

    // out_k_m_1 = out_k;
    // theta_k_m_1 = hw->getThetaM();
    // ref_k_m_1 = ref;

    // return out_k;

    // DISCRETE PID POSITION CONTROL
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 20.58 * (ref - hw->getThetaM()) - 41.06 * (ref_k_m_1 - theta_k_m_1) + 20.48 * (ref_k_m_2 - theta_k_m_2) + 1.667 * out_k_m_1 - 0.6667 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 20.58 * (ref - hw->getThetaM()) - 41.06 * (ref_k_m_1 - theta_k_m_1) + 1.667 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 20.58 * (ref - hw->getThetaM());
    // }

    // out_k_m_2 = out_k_m_1;
    // out_k_m_1 = out_k;

    // ref_k_m_2 = ref_k_m_1;
    // ref_k_m_1 = ref;

    // theta_k_m_2 = theta_k_m_1;
    // theta_k_m_1 = hw->getThetaM();

    // return out_k;

    // ANALOG FILTER
    double err = ref - hw->getThetaM();
    // float tau = analFilter->process(err, hw->getDT());
    float tau = digitalFilter->process(err, hw->getDT());
    return tau;
}

std::vector<std::string> DiscretePosController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
