#include <SI_EFM8BB2_Register_Enums.h>


uint8_t UART_Buffer[10] = {0};
uint8_t UART_Buffer_Size = 0;
uint8_t channel_vtx = 0;
uint8_t channel_vtx_changed = 0;

extern uint8_t CRC8(uint8_t *, uint8_t);

SI_INTERRUPT (UART0_ISR, UART0_IRQn)
{
	uint8_t byte = 0;
	if (SCON0_RI == 1)
	{
		byte = SBUF0;
		if (UART_Buffer_Size == 1 && byte != 0x55)
		{
			UART_Buffer_Size = 0;
			return;
		}
		if (UART_Buffer_Size == 3 && byte > 5)
		{
			UART_Buffer_Size = 0;
			return;
		}
		if (UART_Buffer_Size == 0 && byte == 0xAA ||
				UART_Buffer_Size == 1 && byte == 0x55 ||
				UART_Buffer_Size > 1)
		{
			UART_Buffer[UART_Buffer_Size] = byte;
			UART_Buffer_Size++;
		}	
		if (UART_Buffer_Size > 3 && UART_Buffer_Size == (UART_Buffer[3] + 5))
		{
			UART_Buffer_Size = 0;
			if (UART_Buffer[2] == 0x07 && CRC8(&UART_Buffer, 5) == UART_Buffer[5]
				&& UART_Buffer[4] < 40 && UART_Buffer[4] != channel_vtx)
			{
				channel_vtx = UART_Buffer[4];
				channel_vtx_changed = 1;
			}
		}
		if (UART_Buffer_Size > 9)
			UART_Buffer_Size = 0;
	}
}


