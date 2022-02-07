#include "lpc_types.h"
#include "chip.h"
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

/* Enable Peripheral */
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SSP0);

/* Set clock div to zero and prescaler to 27 for a 2 MHZ clock */
    Chip_SSP_SetClockRate(SSPx,0,27);

/* Set sspx to master mode */  
    Chip_SSP_SetMaster(SSPx,1);
    
/* Set sspx clock polarity, bits per frame and format*/
    Chip_SSP_SetFormat(SSPx, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);     
    
}