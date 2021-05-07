#ifndef INC_DESPOT_HPP
#define INC_DESPOT_HPP

#define UNUSED(x) (void)(x)
#define INLINE inline __attribute__((always_inline))

using void_callback_f = void (*)();

void espurna32RegisterLoop(void_callback_f callback);
void espurna32RegisterReload(void_callback_f callback);
unsigned long espurna32LoopDelay();
void espurna32Reload();

// -----------------------------------------------------------------------------
// std::make_unique backport for C++11
// -----------------------------------------------------------------------------
#if 201103L >= __cplusplus
#include <memory>
using namespace std;
namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif /* 201103L >= __cplusplus */

#include "broker.hpp"
#include "debug.hpp"
#include "helperfn.hpp"
#include "mqtt-broker.hpp"
#include "nvkeyvalue.hpp"
#include "rtcmem32.h"
#include "system.hpp"
#include "sysinfo.hpp"
#include "webserver.hpp"
#include "websockets.hpp"
#include "wifi.hpp"

#endif /* INC_DESPOT_HPP */