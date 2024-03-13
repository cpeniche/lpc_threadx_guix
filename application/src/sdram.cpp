#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "emc_17xx_40xx.h"
#include "sdram.h"
#include "tx_api.h"


SDRAM_Memory::SDRAM_Memory(/* args */)
{
}

SDRAM_Memory::~SDRAM_Memory()
{
}

void SDRAM_Memory::IO_config()
{
  uint32_t index = 0, port = 0, base_addr;
  LPC_IOCON_T base = {0};
  
  /* Configure pins as table pin[] */
  for (index = 0; index < (sizeof(pin) / sizeof(PINMUX_GRP_T)); index++)
  {
    base.p[pin[index].pingrp][pin[index].pinnum] = (LPC_IOCON_BASE + (pin[index].pingrp) * 0x80 + (pin[index].pinnum) * 4);
  }

  Chip_IOCON_SetPinMuxing(&base, pin, sizeof(pin) / sizeof(PINMUX_GRP_T));

  /* Configure port 3 as EMC data pins*/
  port = 3;
  for (index = 0; index < LPC178X_EMC_DATA_PINS; index++)
  {
    base.p[port][index] = (LPC_IOCON_BASE + (port)*0x80 + (index)*4);
    Chip_IOCON_PinMuxSet(&base, port, index, LPC178X_GPIO_EMC_REGVAL);
  }

  /* Configure port 4 as EMC address pins */
  port = 4;
  for (index = 0; index < LPC178X_EMC_ADDR_PINS; index++)
  {
    base.p[port][index] = (LPC_IOCON_BASE + (port)*0x80 + (index)*4);
    Chip_IOCON_PinMuxSet(&base, port, index, LPC178X_GPIO_EMC_REGVAL);
  }

}

void SDRAM_Memory::Init(unsigned int (*delay)(uint32_t time))
{

  uint32_t tmp32;

  /* Clock delay for EMC */

  LPC_SYSCON->EMCDLYCTL = (LPC178X_EMC_CMDDLY << LPC178X_SCC_EMCDLYCTL_CMDDLY_BITS) |
		(LPC178X_EMC_FBCLKDLY << LPC178X_SCC_EMCDLYCTL_FBCLKDLY_BITS);

	 /* Enable EMC*/

  Chip_EMC_Init(1,EMC_CONFIG_ENDIAN_LITTLE);
  
  /* Enable peripheral clock for EMC*/
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);

	/* Address mapping (see Table 133 from the LPC178x/7x User Manual)*/
	 
  LPC_EMC->DYNAMICCONFIG0 = EMC_DYN_CONFIG_8Mx16_4BANKS_12ROWS_9COLS | 1<<14;


	/* Configure DRAM timing*/

  LPC_EMC->DYNAMICRASCAS0 =
		(LPC178X_EMC_RAS << LPC178X_EMC_DYRASCAS_RAS_BITS) |
		(LPC178X_EMC_CAS << LPC178X_EMC_DYRASCAS_CAS_BITS);

  LPC_EMC->DYNAMICREADCONFIG = (LPC178X_EMC_RDCFG_RD << LPC178X_EMC_DYRDCFG_RD_BITS);
  LPC_EMC->DYNAMICRP = LPC178X_EMC_T_RP - 1;
  LPC_EMC->DYNAMICRAS =LPC178X_EMC_T_RAS - 1;
	LPC_EMC->DYNAMICSREX = LPC178X_EMC_T_SREX - 1; 
	LPC_EMC->DYNAMICAPR  = LPC178X_EMC_T_APR - 1; 
	LPC_EMC->DYNAMICDAL  = LPC178X_EMC_T_DAL; 
	LPC_EMC->DYNAMICWR   = LPC178X_EMC_T_WR - 1; 
	LPC_EMC->DYNAMICRC   = LPC178X_EMC_T_RC - 1; 
	LPC_EMC->DYNAMICRFC  = LPC178X_EMC_T_RFC - 1; 
	LPC_EMC->DYNAMICXSR  = LPC178X_EMC_T_XSR - 1; 
	LPC_EMC->DYNAMICRRD  = LPC178X_EMC_T_RRD - 1;  
	LPC_EMC->DYNAMICMRD  = LPC178X_EMC_T_MRD - 1;
	delay(1);
	
	 /* Issue SDRAM NOP (no operation) command*/ 
	
  LPC_EMC->DYNAMICCONTROL =
		LPC178X_EMC_DYCTRL_CE_MSK | LPC178X_EMC_DYCTRL_CS_MSK |
		(LPC178X_EMC_DYCTRL_I_NOP << LPC178X_EMC_DYCTRL_I_BITS);
	delay(1);

	/* Pre-charge all with fast refresh*/
	
  LPC_EMC->DYNAMICCONTROL =
		LPC178X_EMC_DYCTRL_CE_MSK | LPC178X_EMC_DYCTRL_CS_MSK |
		(LPC178X_EMC_DYCTRL_I_PALL << LPC178X_EMC_DYCTRL_I_BITS);
	LPC_EMC->DYNAMICREFRESH = LPC178X_EMC_REFRESH_FAST; 
	delay(1);

	/* Set refresh period*/

  LPC_EMC->DYNAMICREFRESH = LPC178X_EMC_REFRESH;

	/* Load mode word, CAS2, burst of 4*/
	
  LPC_EMC->DYNAMICCONTROL = LPC178X_EMC_DYCTRL_CE_MSK | LPC178X_EMC_DYCTRL_CS_MSK |
		                        (LPC178X_EMC_DYCTRL_I_MODE << LPC178X_EMC_DYCTRL_I_BITS);

/*Send configuration to the sram */

	tmp32 = *(volatile uint32_t *)(0xA0000000 | (LPC178X_EMC_MODEREG_VALUE << LPC178X_EMC_MODEREG_ADDR_SHIFT));

	/* Normal mode */
	
  LPC_EMC->DYNAMICCONTROL =
		(LPC178X_EMC_DYCTRL_I_NORMAL << LPC178X_EMC_DYCTRL_I_BITS);

	/* Enable DRAM buffer*/
	 
  LPC_EMC->DYNAMICCONFIG0 =
  (LPC178X_EMC_AM << LPC178X_EMC_DYCFG_AM_BITS) | LPC178X_EMC_DYCFG_B_MSK;

}


void SDRAM_Memory::Clear(uint32_t *start, uint32_t *end)
{
	uint32_t *pSrc, *pDest;

	pDest = start;

	for ( ; pDest < end ; ) {
	    *pDest++ = 0ul;
	}

}


