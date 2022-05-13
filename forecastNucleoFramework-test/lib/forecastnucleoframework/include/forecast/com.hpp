#ifndef FORECAST_COM_HPP
#define FORECAST_COM_HPP

#include <stdint.h>

namespace forecast {
namespace com {

constexpr uint8_t stx = 0x02;
constexpr uint8_t etx = 0x03;
constexpr uint8_t esc = 0x1b;

enum Type : uint8_t {
    LOG = 0,
    ERROR = 1,
    HANDSHAKE_SYN = 4,
    HANDSHAKE_ACK,
    DATA_REQ,
    DATA,
    MSG
};

enum err {
    UNESCAPED_CHAR = -1,
};

uint32_t escape(const void* src, void* dst, uint32_t srcSz, uint32_t maxSz = 0);

uint32_t unescape(const void* src,
                  void* dst,
                  uint32_t srcSz,
                  uint32_t maxSz = 0);

uint8_t checksum(const uint8_t* src, uint32_t sz);

uint32_t buildPkg(com::Type t,
                  const void* payload,
                  void* dst,
                  uint32_t payloadSz,
                  uint32_t maxSz = 0);

bool checkEtx(const uint8_t* pkg, uint32_t size);

}  // namespace com
}  // namespace forecast

#endif
