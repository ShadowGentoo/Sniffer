#include "../include/stat.h"
#include "inttypes.h"

int searchByIP(const struct interfaceStat * iStat, const uint32_t ipAdress, int * position)
{
    int left = 0;
    int right = iStat->ipCount;
    int middle = 0;
    uint32_t currentIp;

    while (right >= left)
    {
        middle = (right+left)/2;
        currentIp = iStat->statistics->ipAddress;
        if (ipAdress == currentIp)
        {
            * position = middle;
            return middle;
        }
        else
        {
            if (currentIp > ipAdress)
            {
                left = middle + 1;
            }
            else
            {
                right = middle - 1;
            }
        }
    }

    * position = middle;
    return -1;
}