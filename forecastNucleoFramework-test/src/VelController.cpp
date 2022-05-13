#include "../include/VelController.hpp"

using namespace forecast;

VelController::VelController() : Controller(3) {}

VelController::VelController(float kp, float kd, float ki) : Controller(3), kp(kp), kd(kd), ki(ki)
{
    Controller::initialized = true;
}

bool VelController::init(const std::vector<float> &params)
{
    if (params.size() != numberOfParams)
        return false;

    kp = params[0];   // proportional gain
    kd = params[1];   // derivative gain
    ki = params[2];   // integrative gain

    out = 0.0;
    once = true;

    return initialized = true;
}

float VelController::process(const IHardware *hw,
                             float ref,
                             float dref,
                             float ddref)
{
    theta = hw->getThetaM();
    dtheta = hw->getDThetaM();

    /* Initialzation of the first cycle */
    if (once)
    {
        once = false;
        errPast = 0;
        ierr = 0;
    }

    err = ref - dtheta;

    derr = (err - errPast) / hw->getDT();
    errPast = err;
    ierr += err * hw->getDT();

    /* VELOCITY PID CONTROLLER*/
    out = kp * err + kd * derr + ki * ierr;

    //  parameters at lecture:
    //  p_kp:0.5,
    //  p_kd:0.0,
    //  p_ki:0.5,
    //  freq:2000

    return out;
}

std::vector<std::string> VelController::getParamNames() const
{
    return {"KP", "KD", "KI"};
}
