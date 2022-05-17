#include "../include/GravCoulombFrictCompController.hpp"
#include "utility/math.hpp"

using namespace forecast;

GravCoulombFrictCompController::GravCoulombFrictCompController() : Controller(3)
{
}

GravCoulombFrictCompController::GravCoulombFrictCompController(float p_kp, float p_kd, float p_ki) : Controller(3), p_kp(p_kp), p_kd(p_kd), p_ki(p_ki)
{
    Controller::initialized = true;
}

bool GravCoulombFrictCompController::init(const std::vector<float> &params)
{
    if (params.size() != numberOfParams)
        return false;

    p_kp = params[0]; // position proportional
    p_kd = params[1]; // position derivative
    p_ki = params[2]; // position integrative

    out = 0.0;
    return initialized = true;
}

float GravCoulombFrictCompController::process(const IHardware *hw,
                                             float ref,
                                             float dref,
                                             float ddref)
{
    mb = 0.0295;
    f = 0.0174;
    return mb * 9.81 * sin(hw->getThetaM()) + f * utility::sign(hw->getDThetaM());
}

std::vector<std::string> GravCoulombFrictCompController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
