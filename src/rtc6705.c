#include <SI_EFM8BB2_Register_Enums.h>
#include "rtc6705.h"
#include "stdint.h"

extern void delayMSn(uint8_t);

SI_SBIT(MOSI, SFR_P1, 2);
SI_SBIT(NSS, SFR_P1, 3);
SI_SBIT(CLK, SFR_P1, 4);


const uint8_t val_a_list[40] = {
48, 60, 8, 20, 32, 44, 56, 4,
12, 39, 2, 29, 56, 19, 46, 9,
16, 28, 40, 52, 36, 24, 12, 0,
59, 47, 35, 23, 11, 63, 51, 39,
57, 22, 51, 16, 45, 10, 39, 4,
};

const uint16_t val_n_list[40] = {
2293, 2285, 2278, 2270, 2262, 2254, 2246, 2239,
2242, 2249, 2257, 2264, 2271, 2279, 2286, 2294,
2231, 2223, 2215, 2207, 2301, 2309, 2317, 2325,
2244, 2252, 2260, 2268, 2276, 2283, 2291, 2299,
2212, 2227, 2241, 2256, 2270, 2285, 2299, 2314,
}; //these are not real values but fine-tuned tweaked ones

uint8_t CRC8(uint8_t *nice_data, uint8_t len)
{
	unsigned char crc = 0; /* start with 0 so first byte can be 'xored' in */
	unsigned char currByte;
	
	uint8_t i = 0;
	uint8_t j = 0;
	
	for (i = 0; i < len; i++)
	{
		currByte = nice_data[i];

		crc ^= currByte; /* XOR-in the next input byte */

		for (j = 0; j < 8; j++)
		{
			if ((crc & 0x80) != 0)
			{
				crc = (uint8_t)((crc << 1) ^ POLYGEN);
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return crc;
}

void SPIsend(uint32_t command)
{
	uint8_t i = 0;
	
	NSS = 0;
	delayMSn(1);
	for(i = 0; i < 25; ++i)
	{
		MOSI = (command >> i) & 1;
		CLK = 1;
		delayMSn(1);
		CLK = 0;
		delayMSn(1);
	}
	MOSI = 0;
	delayMSn(1);
	NSS = 1;
}

void set_chan(uint8_t chan)
{
	uint8_t val_a;
	uint32_t val_n;
	uint32_t val_hex;
	
	val_a = val_a_list[chan];
	val_n = val_n_list[chan];
	val_hex = RTC6705_REG0_HEAD;
	val_hex |= (RTC6705_REG0_BODY << 5);
	SPIsend(val_hex);
	delayMSn(1);
	val_hex = RTC6705_REG1_HEAD;
	val_hex |= (val_a << 5);
	val_hex |= (val_n << 12);
	SPIsend(val_hex);
}
