#include "lpc_types.h"
#include "chip.h"
#include "tx_api.h"
#include "chip_lpc177x_8x.h"
#include "touch_driver.h"
#include "ssp_17xx_40xx.h"

#define SSPx LPC_SSP0


void Touch_Screen::IO_config()
{
  uint32_t index = 0, port = 0;
  LPC_IOCON_T base = {0};
  
  /* Configure pins as table pin[] */
  for (index = 0; index < (sizeof(pin) / sizeof(PINMUX_GRP_T)); index++)
  {
    base.p[pin[index].pingrp][pin[index].pinnum] = (LPC_IOCON_BASE + (pin[index].pingrp) * 0x80 + (pin[index].pinnum) * 4);
  }

  Chip_IOCON_SetPinMuxing(&base, pin, sizeof(pin) / sizeof(PINMUX_GRP_T));
}


void Touch_Screen::Init()
{

/* Enable Peripheral Clock*/
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SSP0);

/* Set clock div to zero and prescaler to 27 for a 2 MHZ clock */
    Chip_SSP_SetClockRate(SSPx,0,27);

/* Set sspx to master mode */  
    Chip_SSP_SetMaster(SSPx,1);
    
/* Set sspx clock polarity, bits per frame and format*/
    Chip_SSP_SetFormat(SSPx, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);     

/* Enable Peripheral */
    Chip_SSP_Enable(SSPx);
    
}

void Touch_Screen::get_pos(char coordinate)
{
    uint16_t *pos;

    if (coordinate == X_POS)
    {
        tx_buff=0xD3;
        pos = &x_pos;
    }        
    else
    {
        tx_buff=0x93;
        pos = &y_pos;
    }
    Chip_SSP_WriteFrames_Blocking(SSPx,&tx_buff,1);
    tx_thread_sleep(1);
    Chip_SSP_ReadFrames_Blocking(SSPx,&rx_buff[0],2);
    *pos = (*((uint16_t *)rx_buff) >> 4)  & 0x0FF;
}