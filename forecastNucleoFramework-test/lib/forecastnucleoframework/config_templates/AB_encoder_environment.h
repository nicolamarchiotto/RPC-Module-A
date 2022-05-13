#ifndef CONFIG_ABENCODER_ENV_H
#define CONFIG_ABENCODER_ENV_H

namespace forecast {
namespace envEncoder {

constexpr float CPR = $env_ab_encoder[count_per_revolution]$f;
constexpr float GEAR_RATIO = $env_ab_encoder[gear_ratio]$f;

}  // namespace envEncoder
}  // namespace forecast

#endif  // CONFIG_ABENCODER_ENV_H