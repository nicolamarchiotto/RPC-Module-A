
#ifdef TARGET_NUCLEO_L432KC

#include <mbed.h>
#include <forecast/platforms/RPC/RPCApp.hpp>
#include <debug.hpp>
#include "../include/PosController.hpp"
#include "../include/VelController.hpp"
#include "../include/GravCoulombFrictCompController.hpp"
#include "../include/DiscretePosController.hpp"
#include "../include/SwipController.hpp"

double b[5] = {28.03, 34020, 460500, 777700, 0};       // NUM
double a[5] = {1, 2402, 1500000, 69970000, 777700000}; // DEN

float max_current = 4.06;
float kt = 0.231;
float u_sat = max_current * kt;
float scal_factor = 0.5;
float alpha = 0.1;
utility::AnalogFilter *analFilter;

int main()
{

    forecast::RPCApp app;

    // oggetto forecast::RPCHardware gestisce l'hardware della board

    app.setLogger([](float motorRef, const forecast::RPCHardware *hw,
                     const forecast::Controller *motor)
                  { return std::vector<float>{
                        motorRef,
                        hw->getThetaM(),
                        // hw->getDThetaM(),
                        // hw->getDDThetaM(),
                        // hw->getTauM(),
                        // hw->getTauS()
                    }; });

    // Hard-coded reference for the motor
    app.setMotorRefGen([](const forecast::RPCHardware *hw)
                       {
        // float A=1;
        // float f=5;
        
        float ref=0;
        
        static float t = 0.0;

        analFilter= new utility::AnalogFilter(4, a, b);

        // STEP
        // if(t >= 1.0f){
        //     ref = 1.0f;
        // }

        // RAMP
        // static float ramp = 0.0f;
        // ramp += 0.4 * hw->getDT();
        // if (ramp >= 3.5f) {
        //     ramp = 3.5f;
        // }
        // ref = ramp;

        // SIN
        // ref = A*sin(2.0*M_PI*f*t);
        
        float f = alpha * t;
        // float magnitude = analFilter->getMagnitudeHz(f);
        float A = u_sat * scal_factor / hw->getMag(f); //magnitude of the reference swip
        t += hw->getDT();
        ref=A * sin(2.0 * M_PI * f * t); 
        return ref; });

    // Motor controller, in TODO define robot controller
    // forecast::PosController *ctrl=new forecast::PosController();

    app.setMotor(new forecast::SwipController());

    // Handshake with the PC
    app.waitConnection();

    // Require parameters for the controllers which are not already initialized
    app.requireMotorParams();

    // Require the loop frequency
    auto freq = app.requireFloatValue("Loop frequency");

    // Execute control loop, loop frequency
    app.execControlLoop(static_cast<ulong>(freq));
}

#endif // TARGET_STM32F4