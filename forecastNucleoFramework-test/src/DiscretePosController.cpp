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
    once = false;

    // double b[2] = {128.45, 128.45 * 11.7};
    // double a[2] = {1, 6738};


    double b[2] = {0, 1};
    double a[2] = {1, 0};

    analFilter = new utility::AnalogFilter(1, a, b);

    return initialized = true;
}

float DiscretePosController::process(const IHardware *hw,
                                     float ref,
                                     float dref,
                                     float ddref)
{
    // if (once)
    // {
    //     out_k = 47.988 * (ref - hw->getThetaM()) - 47.988 * 0.9942 * (ref_k_m_1 - theta_k_m_1) - 0.255 * out_k_m_1;
    // }
    // else
    // {
    //     once = true;
    //     out_k = 47.988 * (ref - hw->getThetaM());
    // }

    // out_k_m_1 = out_k;
    // theta_k_m_1 = hw->getThetaM();
    // ref_k_m_1 = ref;

    // return out_k;
    double err = ref - hw->getThetaM();
    float tau = analFilter->process(err, hw->getDT());
    return tau;
}

std::vector<std::string> DiscretePosController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
