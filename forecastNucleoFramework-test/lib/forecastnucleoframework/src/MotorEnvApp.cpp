#ifdef TARGET_STM32F4

#include <forecast/config/timer.h>
#include <cmath>

#include <debug.hpp>
#include <forecast/MotorEnvApp.hpp>
#include <forecast/com.hpp>

using namespace forecast;

// private functions for the ticker
MotorEnvApp::MotorEnvApp(unsigned int txBuffSz,
                         unsigned int rxBuffSz,
                         unsigned int baudrate)
    : App(txBuffSz, rxBuffSz, baudrate),
      hw(*this),
      environment(nullptr),
      motor(nullptr),
      envRefGen(nullptr),
      motorRefGen(nullptr) {
    
    // initializing the hardware
    status = hw.init();

    if (status != Status::NO_ERROR) {
        DEBUG_INFO("Hw Initialization error %u\n", static_cast<uint>(status));
        return;
    }
}

bool MotorEnvApp::isOk() {
    return motor and motor->isInitialized() and environment and
           environment->isInitialized() and
           motorRefGen and envRefGen and App::isOk();
}

bool MotorEnvApp::execControl() {
    if (not isOk()) {
        DEBUG_INFO("Error while executing the control!\n");
        std::string msg =
            "Trying to execute the control when the MotorEnvApp is not Ok. "
            "Stats: ";
        msg += "cycle: " + std::to_string(cycleCount) + ", ";
        msg += motor ? "motor is not null" : "motor is null";
        msg += ", ";
        msg += environment ? "environment is not null" : "environment is null";
        msg += ", Status code: ";
        msg += std::to_string(static_cast<uint8_t>(status));

        logError(msg);
        return false;
    }

    hw.update(App::get_ticker_dt() / 1e6f);  // first update values (position,
                                      // velocity, acceleration and force)

    auto envTorque = environment->process(&hw, envRefGen(&hw));
    auto ctrlTorque = motor->process(&hw, motorRefGen(&hw));

    /* Motor control update */
    hw.setControlTorque(ctrlTorque);
    /* Env motor update */
    hw.setEnvTorque(envTorque);

    // hw.update(ctrlTorque.val, motorRefGen(&hw).val, ticker::dt_us / 1e6f);

    cycleCount++;
    return true;
}


void MotorEnvApp::preLoopInit() {
    // enable motor's
    hw.enableControlMotor();
    hw.enableEnvMotor();

    hw.setStartT(us_ticker_read() / 1e6);
}

void MotorEnvApp::loopFinalize() {
    hw.disableControlMotor();
    hw.disableEnvMotor();
}

void MotorEnvApp::logInfo() {
    auto values = logger(motorRefGen(&hw), envRefGen(&hw), &hw, motor.get(),
                         environment.get());
    values.insert(values.begin(), hw.getT());

    sendData(com::Type::LOG, values.data(), values.size() * sizeof(float));
}

void MotorEnvApp::setLogger(LoggerCallback logger) {
    this->logger = std::move(logger);
}

void MotorEnvApp::setMotor(std::unique_ptr<Controller> ctrl) {
    if (ctrl) {
        motor = std::move(ctrl);
    }
}

void MotorEnvApp::setEnvironment(std::unique_ptr<Controller> ctrl) {
    if (ctrl) {
        environment = std::move(ctrl);
    }
}

void MotorEnvApp::setMotor(Controller* ctrl) {
    if (ctrl != nullptr) {
        motor.reset(ctrl);
    }
}

void MotorEnvApp::setEnvironment(Controller* ctrl) {
    if (ctrl != nullptr) {
        environment.reset(ctrl);
    }
}

void MotorEnvApp::setEnvRefGen(RefGen gen) {
    envRefGen = std::move(gen);
}

void MotorEnvApp::setEnvRefSweep(float amplitude,
                                 float frequency_start,
                                 float frequency_end,
                                 float duration) {
    while (std::isnan(amplitude))
        amplitude =
            requireFloatValue("Transparency reference_signal sweep_amplitude");

    while (std::isnan(frequency_start))
        frequency_start = 0.0f;

    while (std::isnan(frequency_end))
        frequency_end =
            requireFloatValue("Transparency reference_signal sweep_max_freq");

    while (std::isnan(duration))
        duration =
            requireFloatValue("Transparency reference_signal sweep_duration");

    setEnvRefGen([amplitude, frequency_start, frequency_end,
                  duration](const forecast::Hardware* hw) {
        float curr_t = hw->getCurrT();
        curr_t = (curr_t > duration) ? duration : curr_t;

        return amplitude *
               sin(2 * M_PI *
                   (frequency_start * curr_t +
                    (((frequency_end - frequency_start) / (2 * duration)) *
                     pow(curr_t, 2))));
    });
}

void MotorEnvApp::setMotorRefGen(RefGen gen) {
    motorRefGen = std::move(gen);
}

void MotorEnvApp::setMotorRefSignal(float amplitude_step_1,
                                    float amplitude_step_2,
                                    float amplitude_step_3,
                                    float amplitude_step_4,
                                    float step_duration,
                                    float sweep_amplitude,
                                    float sweep_duration,
                                    float sweep_max_freq) {
    while (std::isnan(amplitude_step_1))
        amplitude_step_1 =
            requireFloatValue("Tracking reference_signal amplitude_step_1");

    while (std::isnan(amplitude_step_2))
        amplitude_step_2 =
            requireFloatValue("Tracking reference_signal amplitude_step_2");

    while (std::isnan(amplitude_step_3))
        amplitude_step_3 =
            requireFloatValue("Tracking reference_signal amplitude_step_3");

    while (std::isnan(amplitude_step_4))
        amplitude_step_4 =
            requireFloatValue("Tracking reference_signal amplitude_step_4");

    while (std::isnan(step_duration))
        step_duration =
            requireFloatValue("Tracking reference_signal step_duration");

    while (std::isnan(sweep_amplitude))
        sweep_amplitude =
            requireFloatValue("Tracking reference_signal sweep_amplitude");

    while (std::isnan(sweep_duration))
        sweep_duration =
            requireFloatValue("Tracking reference_signal sweep_duration");

    while (std::isnan(sweep_max_freq))
        sweep_max_freq =
            requireFloatValue("Tracking reference_signal sweep_max_freq");

    setMotorRefGen([amplitude_step_1, amplitude_step_2, amplitude_step_3,
                    amplitude_step_4, step_duration, sweep_amplitude,
                    sweep_duration,
                    sweep_max_freq](const forecast::Hardware* hw) {
        float curr_t = hw->getCurrT();
        float ret;

        if (curr_t <= step_duration) {
            ret = (float)amplitude_step_1;
        } else if (curr_t <= 2.0 * step_duration) {
            ret = (float)amplitude_step_2;
        } else if (curr_t <= 3.0 * step_duration) {
            ret = (float)amplitude_step_3;
        } else if (curr_t <= 4.0 * step_duration) {
            ret = (float)amplitude_step_4;
        } else {
            static float t_start = curr_t;
            float curr_t_sweep = curr_t - t_start;

            curr_t_sweep =
                (curr_t_sweep > sweep_duration) ? sweep_duration : curr_t_sweep;

            ret = (float)sweep_amplitude *
                  sin(2 * M_PI *
                      (((sweep_max_freq / (2 * sweep_duration)) *
                        pow(curr_t_sweep, 2))));
        }
        return ret;
    });
}

bool MotorEnvApp::requireMotorParams() {
    DEBUG_INFO("requiring parameters for the motor's controller\n");
    return motor->init(requireControllerParams(motor, "Motor_controller"));
}

bool MotorEnvApp::requireEnvironmentParams() {
    DEBUG_INFO("requiring parameters for the environment's controller\n");
    return environment->init(
        requireControllerParams(environment, "Tracking environment"));
}

#endif // TARGET_STM32F4