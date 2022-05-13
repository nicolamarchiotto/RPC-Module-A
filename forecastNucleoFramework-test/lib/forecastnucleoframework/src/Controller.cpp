#include <forecast/Controller.hpp>

using namespace forecast;

Controller::Controller(size_t numberOfParams) : numberOfParams(numberOfParams) {
    // ntd
}

std::vector<std::string> Controller::getParamNames() const {
    std::vector<std::string> names;
    for (size_t i = 0; i < numberOfParams; ++i) {
        names.emplace_back("param_" + std::to_string(i));
    }
    return names;
}

size_t Controller::getNumberOfParams() const {
    return numberOfParams;
}

bool Controller::isInitialized() const {
    return initialized;
}