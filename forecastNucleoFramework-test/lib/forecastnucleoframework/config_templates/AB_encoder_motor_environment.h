#ifndef CONFIG_ABENCODER_MOTOR_ENV_H
#define CONFIG_ABENCODER_MOTOR_ENV_H

namespace forecast {
namespace envMotorEncoder {

constexpr float CPR = $env_ab_motor_encoder[count_per_revolution]$f;
constexpr float GEAR_RATIO = $env_ab_motor_encoder[gear_ratio]$f;

}  // namespace envMotorEncoder
}  // namespace forecast

#endif  // CONFIG_ABENCODER_MOTOR_ENV_H