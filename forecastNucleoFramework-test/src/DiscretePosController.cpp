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
    return initialized = true;
}

float DiscretePosController::process(const IHardware *hw,
                                     float ref,
                                     float dref,
                                     float ddref)
{

    if (once)
    {
        out_k = 212.5 * (ref - hw->getThetaM()) - 200.3 * (ref_k_m_1 - theta_k_m_1) - 0.5657*out_k_m_1;
    }
    else
    {
        once = true;
        out_k = 212.5 * (ref - hw->getThetaM());
    }

    out_k_m_1 = out_k;
    theta_k_m_1 = hw->getThetaM();
    ref_k_m_1 = ref;

    return out_k;
}

std::vector<std::string> DiscretePosController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
