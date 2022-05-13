#ifndef CONFIG_ABENCODER_MOTOR_H
#define CONFIG_ABENCODER_MOTOR_H

namespace forecast {
namespace motorEncoder {

constexpr float CPR = $motor_ab_encoder[count_per_revolution]$f;
constexpr float GEAR_RATIO = $motor_ab_encoder[gear_ratio]$f;

}  // namespace motorEncoder
}  // namespace forecast

#endif  // CONFIG_ABENCODER_MOTOR_H