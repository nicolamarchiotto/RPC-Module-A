#ifndef CONFIG_ENV_ESCON_MOTOR_H
#define CONFIG_ENV_ESCON_MOTOR_H

// #define E_MOTOR_PWM_PIN D9
// #define E_MOTOR_ENABLE_PIN D12
// #define E_MOTOR_CURRENT_FEEDBACK_PIN A5

namespace envMotor{

    constexpr PinName MOTOR_ENABLE_PIN = $env_escon[enable_pin]$;
    constexpr PinName MOTOR_CURRENT_FEEDBACK_PIN = $env_escon[current_feedback_pin]$;
    constexpr PinName MOTOR_ANALOG_PIN = $env_escon[analog_pin]$;

    // ------- EXO MOTOR ------------------------
    constexpr float MAX_CURR = $env_escon[max_current]$f;
    constexpr float KT = $env_escon[kt]$f;   
    constexpr float JM = $env_escon[jm]$f;
}

#endif // CONFIG_ENV_ESCON_MOTOR_H