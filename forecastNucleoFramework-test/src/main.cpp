
#ifdef TARGET_NUCLEO_L432KC

#include <mbed.h>
#include <forecast/platforms/RPC/RPCApp.hpp>
#include <debug.hpp>
#include "../include/PosController.hpp"
#include "../include/VelController.hpp"
#include "../include/GravCoulombFrictCompController.hpp"
#include "../include/DiscretePosController.hpp"
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
                        hw->getTauM(),
                        // hw->getTauS(),
                    }; });

    // Hard-coded reference for the motor
    app.setMotorRefGen([](const forecast::RPCHardware *hw)
                       {
        float A=M_PI/6;
        float f=5;
        
        float ref=0;
        
        static float t = 0.0;

        // STEP
        if(t >= 1.0f){
            ref = 1.0f;
        }

        // RAMP
        // static float ramp = 0.0f;
        // ramp += 0.4 * hw->getDT();
        // if (ramp >= 3.5f) {
        //     ramp = 3.5f;
        // }
        // ref = ramp;

        // SIN
        // ref = A*sin(2.0*M_PI*f*t);
        
        t += hw->getDT();
        return ref; });

    // Motor controller, in TODO define robot controller
    // forecast::PosController *ctrl=new forecast::PosController();

    app.setMotor(new forecast::DiscretePosController());

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