#include "../include/SwipController.hpp"
#include "utility/math.hpp"

using namespace forecast;

SwipController::SwipController() : Controller(3)
{
}

SwipController::SwipController(float p_kp, float p_kd, float p_ki) : Controller(3), p_kp(p_kp), p_kd(p_kd), p_ki(p_ki)
{
    Controller::initialized = true;
}

bool SwipController::init(const std::vector<float> &params)
{
    if (params.size() != numberOfParams)
        return false;

    p_kp = params[0]; // position proportional
    p_kd = params[1]; // position derivative
    p_ki = params[2]; // position integrative

    // Using a PD controller
    //      28.03 s + 327.9
    // PD = ---------------
    //        s + 1200

    double b_ctrl[2] = {28.03, 327.9}; // NUM
    double a_ctrl[2] = {1, 1200};      // DEN
    analFilterCtrl = new utility::AnalogFilter(1, a_ctrl, b_ctrl);

    return initialized = true;
}

float SwipController::process(const IHardware *hw,
                              float ref,
                              float dref,
                              float ddref)
{
    double err = ref - hw->getThetaM();
    float tau = analFilterCtrl->process(err, hw->getDT());
    return tau;
}

std::vector<std::string> SwipController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
