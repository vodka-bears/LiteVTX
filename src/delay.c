#include "stdint.h"

extern uint8_t channel_vtx_changed;

void delayMS(uint8_t n)
{
	uint8_t i;
	uint32_t j;
	for(i=0;i<n;i++)
	{
		if (channel_vtx_changed)
			return;
		for(j=185;j>0;j--);
	}
}

void delayS(uint8_t n)
{
	if (channel_vtx_changed)
		return;
	delayMS(1000*n);    
}

void delayMSn(uint8_t n)
{
	uint8_t i;
	uint32_t j;
	for(i=0;i<n;i++)
		for(j=185;j>0;j--);     
}



