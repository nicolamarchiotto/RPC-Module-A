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

    double b[5] = {28.03, 34020, 460500, 777700, 0};       // NUM
    double a[5] = {1, 2402, 1500000, 69970000, 777700000}; // DEN

    analFilter = new utility::AnalogFilter(4, a, b);
    max_current = 4.06;
    kt = 0.231;
    u_sat = max_current * kt;
    scal_factor = 0.5;
    alpha = 0.1;
    return initialized = true;
}

float SwipController::process(const IHardware *hw,
                              float ref,
                              float dref,
                              float ddref)
{
    // float f = alpha * t;
    // float magnitude = analFilter->getMagnitudeHz(f);
    // float A = u_sat * scal_factor / magnitude; //magnitude of the reference swip
    // t += hw->getDT();
    

    // //the swip reference for the controller is generated here
    // ref_swip = A * sin(2.0 * M_PI * f * t);

    // ANALOG FILTER CTRL
    double err = ref - hw->getThetaM();
    float tau = analFilterCtrl->process(err, hw->getDT());
    return tau;
}

std::vector<std::string> SwipController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
