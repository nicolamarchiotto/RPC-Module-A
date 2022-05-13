#ifndef CONFIG_ENV_ESCON_MOTOR_H
#define CONFIG_ENV_ESCON_MOTOR_H

// #define E_MOTOR_PWM_PIN D9
// #define E_MOTOR_ENABLE_PIN D12
// #define E_MOTOR_CURRENT_FEEDBACK_PIN A5

namespace envMotor{

    constexpr PinName MOTOR_ENABLE_PIN = PC_11;
    constexpr PinName MOTOR_CURRENT_FEEDBACK_PIN = PA_1;
    constexpr PinName MOTOR_ANALOG_PIN = PA_5;

    // ------- EXO MOTOR ------------------------
    constexpr float MAX_CURR = 2.26f;
    constexpr float KT = 0.231f;   
    constexpr float JM = 0.000506f;
}

#endif // CONFIG_ENV_ESCON_MOTOR_H