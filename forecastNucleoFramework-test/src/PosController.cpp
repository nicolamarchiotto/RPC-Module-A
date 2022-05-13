#include "../include/PosController.hpp"

using namespace forecast;


PosController::PosController() : Controller(3)
{
}

PosController::PosController(float p_kp, float p_kd, float p_ki) : Controller(3), p_kp(p_kp), p_kd(p_kd), p_ki(p_ki)
{
    Controller::initialized = true;
}

bool PosController::init(const std::vector<float> &params)
{
    if (params.size() != numberOfParams)
        return false;

    p_kp = params[0]; // position proportional
    p_kd = params[1]; // position derivative
    p_ki = params[2]; // position integrative

    out = 0.0;
    once = true;

    return initialized = true;
}

float PosController::process(const IHardware *hw,
                             float ref,
                             float dref,
                             float ddref)
{
    theta = hw->getThetaM();

    /* Initialzation of the first cycle */
    if (once)
    {
        once = false;
        errPast = 0;
        ierr = 0;
    }

    err = ref - theta;

    derr = (err - errPast) / hw->getDT();
    errPast = err;
    ierr += err * hw->getDT();

    /* POSITION PID CONTROLLER*/
    out = p_kp * err + p_kd * derr + p_ki * ierr;

    // parameters at lecture:
    //  p_kp:5,
    //  p_kd:0.25,
    //  p_ki:0.1,
    //  freq:2000

    return out;
}

std::vector<std::string> PosController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
