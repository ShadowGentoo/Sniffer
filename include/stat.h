#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include "inttypes.h"

struct ipStat {
    uint32_t ipAddress;
    uint32_t count;
};

struct interfaceStat {
    struct ipStat * statistics;
    uint32_t ipCount;
};

#endif