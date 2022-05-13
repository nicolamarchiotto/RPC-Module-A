#ifndef CONFIG_CONTROL_ESCON_MOTOR_H
#define CONFIG_CONTROL_ESCON_MOTOR_H

// #define C_MOTOR_ENABLE_PIN D12
// #define C_MOTOR_CURRENT_FEEDBACK_PIN A5
// #define C_MOTOR_ANALOG_PIN A3

namespace motorControl {

    constexpr PinName MOTOR_ENABLE_PIN = D12;
    constexpr PinName MOTOR_CURRENT_FEEDBACK_PIN = A3;
    constexpr PinName MOTOR_ANALOG_PIN = A4;

    // ------- EXO MOTOR ------------------------
    constexpr float MAX_CURR = 4.06f;
    constexpr float KT = 0.231f;   
    constexpr float JM = 0.0121716f;
    constexpr float DM = 0.4f;

}

#endif // CONFIG_CONTROL_ESCON_MOTOR_H