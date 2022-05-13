#ifndef CONFIG_CONTROL_ESCON_MOTOR_H
#define CONFIG_CONTROL_ESCON_MOTOR_H

// #define C_MOTOR_ENABLE_PIN D12
// #define C_MOTOR_CURRENT_FEEDBACK_PIN A5
// #define C_MOTOR_ANALOG_PIN A3

namespace motorControl {

    constexpr PinName MOTOR_ENABLE_PIN = $motor_escon[enable_pin]$;
    constexpr PinName MOTOR_CURRENT_FEEDBACK_PIN = $motor_escon[current_feedback_pin]$;
    constexpr PinName MOTOR_ANALOG_PIN = $motor_escon[analog_pin]$;

    // ------- EXO MOTOR ------------------------
    constexpr float MAX_CURR = $motor_escon[max_current]$f;
    constexpr float KT = $motor_escon[kt]$f;   
    constexpr float JM = $motor_escon[jm]$f;
    constexpr float DM = $motor_escon[dm]$f;

}

#endif // CONFIG_CONTROL_ESCON_MOTOR_H