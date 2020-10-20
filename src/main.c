#include <SI_EFM8BB2_Register_Enums.h>
#include "stdint.h"
#include "rtc6705.h"
#include <absacc.h>
#define FLASH_ADDR 0x1000


SI_SBIT(LED, SFR_P1, 0);

extern uint8_t UART_Buffer[12];
extern uint8_t channel_vtx;
extern uint8_t channel_vtx_changed;
extern void delayS(uint8_t);
extern void delayMS(uint8_t);
extern void set_chan(uint8_t);
extern void enter_DefaultMode_from_RESET(void);

uint8_t read_chan_from_mem()
{
	uint8_t chan;
	IE_EA = 0;
	chan = CBYTE[FLASH_ADDR];
	if (chan > 39)
		chan = 0;
	IE_EA = 1;
	return chan;
}

void write_chan_to_mem(uint8_t chan)
{
	IE_EA = 0;
	VDM0CN = 0x80;
	RSTSRC = 0x02;
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL = 3;
	XBYTE[FLASH_ADDR] = chan;
	PSCTL = 0;
	FLKEY = 0xA5;
	FLKEY = 0xF1;
	PSCTL = 1;
	XBYTE[FLASH_ADDR] = chan;
	PSCTL = 0;
	IE_EA = 1;
}


void main(void)
{
	uint8_t i = 0;
	uint8_t band = 0;
	uint8_t chan = 0;
  enter_DefaultMode_from_RESET();
	
	delayMS(150);
	channel_vtx = read_chan_from_mem();
	set_chan(channel_vtx);
	SCON0 |= SCON0_REN__RECEIVE_ENABLED;
	LED = 1;

  while(1)
  {
		if (channel_vtx_changed)
		{
			write_chan_to_mem(channel_vtx);
			set_chan(channel_vtx);
			channel_vtx_changed = 0;
		}
		band = channel_vtx / 8;
		chan = channel_vtx % 8;
		delayS(2);
		if (band != 4)
		{
			for (i=0;i<band+1;++i)
			{
				if (channel_vtx_changed)
					break;
				LED = 0;
				delayS(1);
				LED = 1;
				delayMS(300);
			}
		}
		for (i=0;i<chan+1;++i)
		{
			if (channel_vtx_changed)
					break;
			LED = 0;
			delayMS(300);
			LED = 1;
			delayMS(300);
		}
		
  }
}

