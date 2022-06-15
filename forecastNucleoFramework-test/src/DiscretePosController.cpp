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

    //  PD(s)
    //  double b[2] = {25.641, 25.641*11.7};
    //  double a[2] = {1, 1200};
    //  analFilter = new utility::AnalogFilter(1, a, b);

    //  PD(z)
    //  double b_z[2] = { 19.78, -19.67};
    //  double a_z[2] = {1, -0.5385};
    //  digitalFilter = new utility::DigitalFilter(2, a_z, b_z);



    //  PID_high_dist(s)
    //  double b[3] = {5.206, 75.93, 55.09}; // NUM
    //  double a[3] = {1, 338.3, 0};           // DEN
    //  analFilter = new utility::AnalogFilter(2, a, b);

    //  PID_high_dist(z)
    //  double b_z[3] = {4.818, -9.6, 4.783}; // NUM
    //  double a_z[3] = {1, -1.844, 0.844};    // DEN
    //  digitalFilter = new utility::DigitalFilter(2, a_z, b_z);



    //  PID_low_dist(s)
     double b[3] = {24.64, 253.4, 646.4}; // NUM
     double a[3] = {1, 800, 0};           // DEN
     analFilter = new utility::AnalogFilter(2, a, b);

    //  PID_low_dist(z)
     double b_z[3] = {20.58, -41.06, 20.48}; // NUM
     double a_z[3] = {1, -1.667, 0.6667};    // DEN
     digitalFilter = new utility::DigitalFilter(2, a_z, b_z);

    return initialized = true;
}

float DiscretePosController::process(const IHardware *hw,
                                     float ref,
                                     float dref,
                                     float ddref)
{
    // DISCRETE PD POSITION CONTROL Tustin a 2000 Hz

    // if (once)
    // {
    //     out_k = 19.78 * (ref - hw->getThetaM()) - 19.67 * (ref_k_m_1 - theta_k_m_1) + 0.5385 * out_k_m_1;
    // }
    // else
    // {
    //     once = true;
    //     out_k = 19.78 * (ref - hw->getThetaM());
    // }

    // out_k_m_1 = out_k;
    // theta_k_m_1 = hw->getThetaM();
    // ref_k_m_1 = ref;
    // return out_k;


    // DISCRETE PID POSITION CONTROL High Disturbance Tustin a 2000 Hz
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 4.818 * (ref - hw->getThetaM()) - 9.6 * (ref_k_m_1 - theta_k_m_1) + 4.783 * (ref_k_m_2 - theta_k_m_2) + 1.844 * out_k_m_1 - 0.844 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 4.818 * (ref - hw->getThetaM()) - 9.6 * (ref_k_m_1 - theta_k_m_1) + 1.844 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 4.818 * (ref - hw->getThetaM());
    // }


    // DISCRETE PID POSITION CONTROL Low Disturbance Tustin a 2000 Hz
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

    // DISCRETE PID POSITION CONTROL Low Disturbance ZoH a 2000 Hz, non arriva a ref ma costante a regime 0.7 ref
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 24.64 * (ref - hw->getThetaM()) - 49.17 * (ref_k_m_1 - theta_k_m_1) + 24.53 * (ref_k_m_2 - theta_k_m_2) + 1.67 * out_k_m_1 - 0.6703 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 24.64 * (ref - hw->getThetaM()) - 49.17 * (ref_k_m_1 - theta_k_m_1) + 1.67 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 24.64 * (ref - hw->getThetaM());
    // }

    // DISCRETE PID POSITION CONTROL Low Disturbance FoH a 2000 Hz, non arriva a ref ma costante a regime 0.7 ref
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 20.36 * (ref - hw->getThetaM()) - 40.62 * (ref_k_m_1 - theta_k_m_1) + 20.26 * (ref_k_m_2 - theta_k_m_2) + 1.67 * out_k_m_1 - 0.6703 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 20.36 * (ref - hw->getThetaM()) - 40.62 * (ref_k_m_1 - theta_k_m_1) + 1.67 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 20.36 * (ref - hw->getThetaM());
    // }

    // DISCRETE PID POSITION CONTROL Low Disturbance Impulse a 2000 Hz, instabile
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = -9.728 * (ref - hw->getThetaM()) + 9.729 * (ref_k_m_1 - theta_k_m_1) + 0 * (ref_k_m_2 - theta_k_m_2) + 1.67 * out_k_m_1 - 0.6703 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = -9.728 * (ref - hw->getThetaM()) + 9.729 * (ref_k_m_1 - theta_k_m_1) + 1.67 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = -9.728 * (ref - hw->getThetaM());
    // }

    // DISCRETE PID POSITION CONTROL Low Disturbance matched a 2000 Hz,  non arriva a ref ma costante a regime 0.7 ref
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 20.36 * (ref - hw->getThetaM()) - 40.61 * (ref_k_m_1 - theta_k_m_1) + 20.25 * (ref_k_m_2 - theta_k_m_2) + 1.67 * out_k_m_1 - 0.6703 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 20.36 * (ref - hw->getThetaM()) - 40.61 * (ref_k_m_1 - theta_k_m_1) + 1.67 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 20.36 * (ref - hw->getThetaM());
    // }

    // DISCRETE PID POSITION CONTROL Low Disturbance Least Squared a 2000 Hz, instabile
    // if (once)
    // {
    //     if (twice)
    //     {
    //         out_k = 20.41 * (ref - hw->getThetaM()) - 40.71 * (ref_k_m_1 - theta_k_m_1) + 20.03 * (ref_k_m_2 - theta_k_m_2) + 1.657 * out_k_m_1 - 0.6573 * out_k_m_2;
    //     }
    //     else
    //     {
    //         twice = true;
    //         out_k = 20.41 * (ref - hw->getThetaM()) - 40.71 * (ref_k_m_1 - theta_k_m_1) + 1.657 * out_k_m_1;
    //     }
    // }
    // else
    // {
    //     once = true;
    //     out_k = 20.41 * (ref - hw->getThetaM());
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
    float tau = analFilter->process(err, hw->getDT());
    return tau;

    // DIGITAL FILTER
    // double err = ref - hw->getThetaM();
    // float tau = digitalFilter->process(err, hw->getDT());
    // return tau;
}

std::vector<std::string> DiscretePosController::getParamNames() const
{
    return {"p_KP", "p_KD", "p_KI"};
}
