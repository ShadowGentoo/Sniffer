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

int searchByIP(const struct interfaceStat * iStat, const uint32_t ipAdress, int * position);
void readStat(struct interfaceStat * iStat, const char * filename);
void writeStat(struct interfaceStat * iStat, const char * filename);
void addToStat(struct interfaceStat * ifStat, uint32_t address);

#endif