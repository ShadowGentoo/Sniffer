#include "../include/stat.h"
#include "inttypes.h"
#include "stdio.h"
#include "stdlib.h"

void readStat(struct interfaceStat * iStat, const char * filename)
{
    FILE * statFile = fopen(filename, "r");
    int result = 0;

    if (statFile != NULL)
    {
        result = fread(&(iStat->ipCount), sizeof(iStat->ipCount), 1, statFile);
        if (result != 1)
        {
            printf("Readings error!\n");
        }
        int freeMemory = 10 - (iStat->ipCount % 10);
        iStat->statistics = malloc(sizeof(struct ipStat) * (iStat->ipCount + freeMemory));
        
        result = fread(iStat->statistics, sizeof(struct ipStat), iStat->ipCount, statFile);
        if (result != iStat->ipCount)
        {
            printf("Readings error!\n");
        }

        fclose(statFile);
    }
    else
    {
        printf("Failed to open file %s for reading\n", filename);
    }
    
}

void writeStat(struct interfaceStat * iStat, const char * filename)
{
    FILE * statFile = fopen(filename, "w");
    
    if (statFile != NULL)
    {
        fwrite(&(iStat->ipCount), sizeof(iStat->ipCount), 1, statFile);
        fwrite(iStat->statistics, sizeof(struct ipStat), iStat->ipCount, statFile);

        fclose(statFile);
    }
    else
    {
        printf("Failed to open file %s for writing\n", filename);
    }
}

int searchByIP(const struct interfaceStat * iStat, const uint32_t ipAdress, int * position)
{
    int left = 0;
    int right = iStat->ipCount-1;
    int middle = 0;
    uint32_t currentIp;

    while (right >= left)
    {
        middle = (right+left)/2;
        currentIp = iStat->statistics[middle].ipAddress;
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

    if (iStat->statistics[middle].ipAddress < ipAdress)
    {
        * position = middle + 1;
    }
    else
    {
        * position = middle;
    }
    
    return -1;
}

void addToStat(struct interfaceStat * ifStat, uint32_t address)
{
	int pos = 0;

	if (ifStat->ipCount == 0)
	{
		ifStat->statistics = (struct ipStat *) malloc(sizeof(struct ipStat)*10);
		if (ifStat->statistics == NULL)
		{
			printf("Failed to allocate memory for new interface statistics structure");
			exit(1);
		}
        ifStat->ipCount++;
        ifStat->statistics[0].count = 1;
        ifStat->statistics[0].ipAddress = address;
    }
    else
    {
        if(searchByIP(ifStat, address, &pos) != -1)
	    {
		    ifStat->statistics[pos].count++;
	    }
	    else
	    {
    		//insert new record
		    ifStat->ipCount++;
	    	if (ifStat->ipCount % 9 == 0)
    		{
		    	ifStat->statistics = (struct ipStat *) realloc(ifStat->statistics, sizeof(struct ipStat)*(ifStat->ipCount+10));
	    		if (ifStat->statistics == NULL)
    			{
				    printf("Failed to reallocate memory for new interface statistics structure");
				    exit(1);
			    }			
		    }

            //very ineffective. Need fo find another way.
            for (uint32_t i = ifStat->ipCount; i > pos; i--)
            {
                ifStat->statistics[i+1] = ifStat->statistics[i];
            }
            ifStat->statistics[pos].count = 1;
            ifStat->statistics[pos].ipAddress = address;
	    }	
    }
}