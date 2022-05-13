#ifndef TARGET_STM32F4

#include <forecast/platforms/RPC/RPCApp.hpp>

using namespace forecast;

#include <debug.hpp>

// private functions for the ticker
RPCApp::RPCApp(unsigned int txBuffSz,
               unsigned int rxBuffSz,
               unsigned int baudrate)
    : App(txBuffSz, rxBuffSz, baudrate),
      hw(*this),
      motor(nullptr),
      motorRefGen(nullptr) {
    // initializing the hardware
    status = hw.init();

    if (status != Status::NO_ERROR) {
        DEBUG_INFO("Hw Initialization error %u\n", static_cast<uint>(status));
        return;
    }
}

void RPCApp::logInfo() {
    auto values = logger(motorRefGen(&hw), &hw, motor.get());
    values.insert(values.begin(), hw.getT());

    sendData(com::Type::LOG, values.data(), values.size() * sizeof(float));
}


void RPCApp::setLogger(LoggerCallback logger) {
    this->logger = std::move(logger);
}

void RPCApp::setMotor(std::unique_ptr<Controller> ctrl) {
    if (ctrl) {
        motor = std::move(ctrl);
    }
}

void RPCApp::setMotor(Controller* ctrl) {
    if (ctrl != nullptr) {
        motor.reset(ctrl);
    }
}

void RPCApp::setMotorRefGen(RefGen gen) {
    motorRefGen = std::move(gen);
}

bool RPCApp::isOk() {
    return App::isOk() and motor and motor->isInitialized() and motorRefGen;
}

bool RPCApp::execControl() {
    if (not isOk()) {
        DEBUG_INFO("Error while executing the control!\n");
        std::string msg =
            "Trying to execute the control when the MotorEnvApp is not Ok. "
            "Stats: ";
        msg += "cycle: " + std::to_string(cycleCount) + ", ";
        msg += motor ? "motor is not null" : "motor is null";
        msg += ", Status code: ";
        msg += std::to_string(static_cast<uint8_t>(status));

        logError(msg);
        return false;
    }

    hw.update(App::get_ticker_dt() / 1e6f);  // first update values (position,
                                      // velocity, acceleration and force)

    auto ctrlTorque = motor->process(&hw, motorRefGen(&hw));

    /* Motor control update */
    hw.setControlTorque(ctrlTorque);

    cycleCount++;
    return true;
}

void RPCApp::preLoopInit() {
    // enable motor's
    hw.enableControlMotor();
}

void RPCApp::loopFinalize() {
    hw.disableControlMotor();
}

bool RPCApp::requireMotorParams() {
    DEBUG_INFO("requiring parameters for the motor's controller\n");
    return motor->init(requireControllerParams(motor, "Motor_controller"));
}

#endif  // TARGET_STM32F4