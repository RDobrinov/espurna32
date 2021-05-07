#ifndef _RTCMEM32_H_
#define _RTCMEM32_H_

//#include <Arduino.h>

#define RTCMEM_MAGIC 0x45535075

struct RtcmemData {
    uint32_t magic;
    uint32_t sys;
    uint32_t relay;
    uint32_t mqtt;
    uint64_t light;
    double energy[4];
};

void _rtcmemErase();
void _rtcmemInit();
bool _rtcmemStatus();
bool rtcmemStatus();
void rtcmemSetup();

extern RtcmemData *Rtcmem;
#endif