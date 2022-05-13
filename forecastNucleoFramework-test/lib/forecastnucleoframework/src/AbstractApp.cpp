#include <forecast/config/timer.h>
#include <forecast/AbstractApp.hpp>
#include <forecast/com.hpp>
#include <forecast/Controller.hpp>

#include <debug.hpp>

using namespace forecast;

namespace ticker {

// variables and structure for creating the ticker
volatile bool flag;
volatile uint32_t dt_us;  // real dt in micro seconds

TIM_HandleTypeDef mTimUserHandle;

static void interrupt_handler() {
    static uint32_t lastCallTime;
    if (__HAL_TIM_GET_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE);
        flag = true;
        auto time = us_ticker_read();
        dt_us = time - lastCallTime;
        lastCallTime = time;
    }
}

static bool init(float freq) {
    ENABLE_TICKER();

    uint32_t prescaler = 100;
    uint32_t period =
        static_cast<uint32_t>((float)TICKER_CLOCK_FREQ / prescaler / freq);

    while (period > 65536) {
        DEBUG_INFO("period too high try to find a valid prescaler\n");
        if (++prescaler > 65535) {
            DEBUG_INFO("cannot find a valid prescaler for the frequency %f\n",
                       freq);
            return false;
        }
        period =
            static_cast<uint32_t>((float)TICKER_CLOCK_FREQ / prescaler / freq);
    }

    while (period < 1) {
        DEBUG_INFO("period too low try to find a valid prescaler\n");
        if (--prescaler < 1) {
            DEBUG_INFO("cannot find a valid prescaler for the frequency %f\n",
                       freq);
            return false;
        }
        period =
            static_cast<uint32_t>((float)TICKER_CLOCK_FREQ / prescaler / freq);
    }

    mTimUserHandle.Instance = TICKER_HW_TIMER;
    mTimUserHandle.Init.Prescaler = prescaler - 1;
    mTimUserHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    mTimUserHandle.Init.Period = period - 1;
    mTimUserHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&mTimUserHandle);
    HAL_TIM_Base_Start_IT(&mTimUserHandle);

    HAL_NVIC_SetPriority(TICKER_HW_TIMER_IRQn, 2, 0);
    NVIC_SetVector(TICKER_HW_TIMER_IRQn, (uint32_t)interrupt_handler);
    NVIC_EnableIRQ(TICKER_HW_TIMER_IRQn);

    flag = false;

    return true;
}
};  // namespace ticker

// implementation of the App object

App::App(unsigned int txBuffSz, unsigned int rxBuffSz, unsigned int baudrate)
    : pc(USBTX,
         USBRX,
         C_8bits,
         C_Use_RX_TX,
         baudrate,
         C_Parity_None,
         C_1stop,
         C_RX_TX_DMA,
         txBuffSz,
         rxBuffSz),
    //  : pc(USBTX, USBRX, 921600, C_TX_DMA, 90, 0),
      txBuffSz(txBuffSz),
      rxBuffSz(rxBuffSz),
      handshake(false),
      cycleCount(0),
      status(Status::NO_ERROR) {
    
    DEBUG_INFO("Trying to enable the communication:\n");
    // enabling the communication
    auto usartStatus = pc.enable();
    if (usartStatus != USART_status::S_ok) {
        DEBUG_INFO("Cannot initialize the exit code of enable function is %u",
                   usartStatus);
        status = Status::COMMUNICATION_ERROR;
        return;
    }

    
    DEBUG_INFO("\tpc enabled\n");

    usartStatus = pc.transmit_enable();
    if (usartStatus != USART_status::S_ok) {
        DEBUG_INFO(
            "Cannot enable the transmit functionalitye exit code of "
            "transmit_enable function is %u",
            usartStatus);
        status = Status::COMMUNICATION_ERROR;
        return;
    }
    DEBUG_INFO("\ttransmit enabled\n");

    usartStatus = pc.receive_enable();
    if (usartStatus != USART_status::S_ok) {
        DEBUG_INFO(
            "Cannot enable the receive functionalitye exit code of "
            "receive_enable function is %u",
            usartStatus);
        status = Status::COMMUNICATION_ERROR;
        return;
    }

    DEBUG_INFO("\treceive enabled\n");

}

uint32_t App::get_ticker_dt() const {
    return ticker::dt_us;
}

void App::logError(const std::string& msg) {
    sendData(com::Type::ERROR, msg.c_str(), msg.size() * sizeof(char) + 1);
}

float App::requireFloatValue(const std::string& valueName) {
    DEBUG_INFO("Requiring floating point value\n\tsending request...\n\t");
    sendData(com::Type::DATA_REQ, valueName.c_str(),
             valueName.size() * sizeof(char) + 1 /*for the \0 char*/, true);

    float data;
    com::Type t;

    auto sz = receiveData(t, &data, sizeof(float));
    if (sz != sizeof(float)) {
        DEBUG_INFO("Cannot receive float value, buff size: %l\n", sz);
        return NAN;
    }

    if (t != com::Type::DATA) {
        DEBUG_INFO("Invalid package type received while waiting data: %u\n",
                   static_cast<uint>(t));
        return NAN;
    }

    return data;
}

void App::sendMessage(const std::string& msg) {
    sendData(com::Type::MSG, msg.c_str(), msg.size() * sizeof(char) + 1, true);
}

bool App::waitConnection() {
    // first check to avoid buffers errors:
    pc.flush_tx_buffer();
    pc.flush_rx_buffer();
    // fisr im going to receive a syn pkg
    com::Type t;
    // if I have errors I'm going to retry
    if (receiveData(t, nullptr) < 0)
        return false;

    if (t != com::Type::HANDSHAKE_SYN)
        return false;

    // if it's an handshake syn I send an handshake ack and return true
    sendData(com::HANDSHAKE_ACK, nullptr, 0, true);
    return this->handshake = true;
}

bool App::isOk() {
    return status == Status::NO_ERROR and handshake;
}

bool App::execControlLoop(unsigned long freq) {
    if (not ticker::init(freq)) {
        DEBUG_INFO("cannot intialize the ticker for the desired freq\n");
        return false;
    }

    DEBUG_INFO("Executing the control loop at frequency %luHz\n", freq);

    preLoopInit();  // allow the user to inizialize some values exactly before
                    // the loop stated.
    while (true) {
        while (not ticker::flag)
            ;  // wait for the ticker to be ready
        ticker::flag = false;

        if (pc.is_data_available() == S_data_available) {
            // if the user send data I interrupt the cycle
            // N.B. You can block the cycle with any package, but I'm going to
            //      consume that package expecting it to be a pause command
            com::Type t;
            receiveData(t, nullptr);

            loopFinalize();  // if the user wants to do something...

            wait_ms(500);
            HAL_NVIC_SystemReset();
        }

        if (not execControl()) {
            DEBUG_INFO("cannot exec the control\n");

            loopFinalize();  // safety first

            return false;
        }

        logInfo();  // logging part

        if (ticker::flag) {
            logError("Cannot keep the loop frequency: " + std::to_string(freq) +
                     ". Please manually reset the board!");
            status = Status::FREQ_NOT_KEPT;

            loopFinalize();  // safety

            // A wait is needed to print the error message, otherwise the nucleo
            // just stops the execution
            wait_ms(500);
            return false;
        }
    }
}

void App::fatalError(const std::string& msg) {
    sendData(com::Type::ERROR, msg.c_str(), msg.size() * sizeof(char) + 1);

    wait_ms(1000);  // waiting one second

    std::abort();
}

void App::sendData(com::Type t, const void* data, size_t size, bool wait) {
    auto* pkg = pc.get_tx_buffer_pointer(true);

    auto sz = com::buildPkg(t, data, pkg, size, txBuffSz);

    pc.send(sz, wait);  // waiting for the message to be sent
}

ssize_t App::receiveData(com::Type& t, void* buff, size_t maxSz) {
    // creating the buffer and initializing at 0.
    auto pkg = new uint8_t[rxBuffSz];
    for (size_t i = 0; i < rxBuffSz; ++i)
        pkg[i] = 0u;

    // receiving data
    size_t i = 0;
    DEBUG_INFO("waiting for receive data... ");
    while (not com::checkEtx(pkg, i)) {
        while ((pc.is_data_available()) == S_data_unavailable)
            ;  // wait

        auto status = pc.get_rx_buffer(&pkg[i]);

        if (status == S_ok or status == S_rx_buffer_empty) {
            DEBUG_INFO("STATUS Ok %d\n", pkg[i]);
            // I've received a byte of data
            i++;
        } else if (status == S_data_unavailable) {
            continue;  // Compatibility for using read without dma
        } else if (status == S_rx_buffer_already_empty) {
            DEBUG_INFO("NOT YET DATA\n");
            continue;  // Compatitbility for using read with dma
        } else {
            // I'm in a error case something went wrong
            DEBUG_INFO("\n\nERROR aborting i = %d, status = %d\n\n", i, status);
            return -1;
        }

        if (i >= rxBuffSz) {
            DEBUG_INFO("\n\nERROR aborting i = %d > rxBuffSz%d\n\n", i,
                       rxBuffSz);
            // return error out of dimension
            return -2;
        }
    }

    // checking the pkg, I don't need to check for etx because it's done in the
    // previous while loop
    if (pkg[0] != com::stx) {
        logError("received a malformed package");
        DEBUG_INFO("Error: no stx\n\tpackage: [");
        size_t i = 0;
        while (not com::checkEtx(pkg, i) and i < rxBuffSz) {
            DEBUG_INFO("%u ", static_cast<uint>(pkg[i++]));
        }
        for (size_t j = 0; j < 3 and i < rxBuffSz; ++j) {
            DEBUG_INFO("%u ", static_cast<uint>(pkg[i++]));
        }
        DEBUG_INFO("\b]\n");
        return -3;
    }
    DEBUG_INFO("received!\n");

    t = static_cast<com::Type>(pkg[1]);  // returning the type

    // returning the data.
    ssize_t sz = 0;
    if (buff != nullptr)
        sz = com::unescape(&pkg[2], buff, i - 3, maxSz);

    delete pkg;  // deleting pkg for avoid memory leaks

    return sz;
}


std::vector<float> App::requireControllerParams(
    const std::unique_ptr<Controller>& ctrl,
    const std::string& ctrlName) {
    std::vector<float> params;
    for (const auto& pname : ctrl->getParamNames()) {
        float value = NAN;
        while (std::isnan(value)) {
            value = requireFloatValue(ctrlName + " " + pname);
            if (std::isnan(value)) {
                DEBUG_INFO("Received nan value... retrying!\n");
                logError("Reveived NAN value for the parameter: " + pname);
            }
        }
        params.emplace_back(value);
    }

    return params;
}