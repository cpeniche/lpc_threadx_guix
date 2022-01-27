#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "emc_17xx_40xx.h"
/*
 * SCC registers
 */
/*
 * EMC Delay Control register
 */
/* Programmable delay value for EMC outputs in command delayed mode */
#define SCC_EMCDLYCTL_CMDDLY_BITS   0
/*
 * Programmable delay value for the feedback clock that controls input data
 * sampling
 */
#define SCC_EMCDLYCTL_FBCLKDLY_BITS 8

/*
 * SDRAM-specific configuration
 */
/*
 * Programmable delay value for EMC outputs in command delayed mode
 */
#define LPC178X_EMC_CMDDLY  0x10
/*
 * Programmable delay value for the feedback clock that controls input data
 * sampling
 */
#define LPC178X_EMC_FBCLKDLY    0x10
/*
 * K4M513233 SDRAM: 32-bit, 4 banks, 13 rows, 9 cols.
 * See Table 133 "Address mapping" in the LPC178x/7x User Manual.
 */
#define LPC178X_EMC_AM      0x8D

/* Address mapping */
#define LPC178X_EMC_DYCFG_AM_BITS   7

/* First Sram Address */
#define SRAM_ADDRESS 0xA0000000

#define LPC178X_EMC_MODEREG_BL      2   /* Burst Length code */
#define LPC178X_EMC_MODEREG_CAS     2   /* CAS Latency */

/*
 * The SDRAM chip (K4M513233) mode register.
 * See K4M513233 datasheet
 */
#define LPC178X_EMC_MODEREG_BL_BITS     0   /* Burst Length */
#define LPC178X_EMC_MODEREG_CAS_BITS    4   /* CAS Latency */

#define LPC178X_EMC_MODEREG_VALUE \
    ((LPC178X_EMC_MODEREG_BL << LPC178X_EMC_MODEREG_BL_BITS) | \
    (LPC178X_EMC_MODEREG_CAS << LPC178X_EMC_MODEREG_CAS_BITS))

/*
 * Offset of the 12 least-significant bits of mode register (A0..A11)
 * in addresses on the AHB bus.
 *
 * In the high-performance mode the shift should be the following:
 * 13 = 9 (column bits) + 2 (bank select bits) + 2 (32 bits)
 *    1. K4M513233 SDRAM has 512 columns, therefore 9 bits are used for the column number.
 *    2. Bank select field has 2 bits.
 *    3. `2` is log2(32/8), because the SDRAM chip is 32-bit, and its
 *        internal addresses do not have 2 least-significant bits of
 *        the AHB bus addresses.
 *
 * In the low-power mode this shift will be different.
 */

#define LPC178X_GPIO_EMC_REGVAL \
	(FUNC1 | IOCON_MODE_INACT | IOCON_FASTSLEW_EN)



const PINMUX_GRP_T pin[] ={
  /* Configure EMC bank address select 0 and 1 (BA0, BA1) */
	{4, 13, LPC178X_GPIO_EMC_REGVAL},
	{4, 14, LPC178X_GPIO_EMC_REGVAL},

	/* Configure EMC column address strobe (CAS) */
	{2, 16, LPC178X_GPIO_EMC_REGVAL},
	/* Configure EMC row address strobe (RAS) */
	{2, 17, LPC178X_GPIO_EMC_REGVAL},

	/* Configure EMC write enable (WE) */
	{4, 25, LPC178X_GPIO_EMC_REGVAL},

	/* Configure EMC clock input (CLK) */
	{2, 18, LPC178X_GPIO_EMC_REGVAL},
	/* Configure EMC clock enable (CKE) */
	{2, 24, LPC178X_GPIO_EMC_REGVAL},

	/* Configure EMC chip select (DYCS0) */
	{2, 20, LPC178X_GPIO_EMC_REGVAL},

	/* Configure EMC I/O mask (DQM0..DQM3) */
	{2, 28, LPC178X_GPIO_EMC_REGVAL},
	{2, 29, LPC178X_GPIO_EMC_REGVAL},
	{2, 30, LPC178X_GPIO_EMC_REGVAL},
	{2, 31, LPC178X_GPIO_EMC_REGVAL},

};

IP_EMC_DYN_CONFIG_T Sram_Default_Config  =
{
  .RefreshPeriod  = 0x1D,    /*!< Refresh period 7.8us/row */
  .ReadConfig     = 0x01,    /*!< Clock*/
  .tRP            = 9,    /*!< Precharge Command Period */
  .tRAS           = 25,    /*!< Active to Precharge Command Period */
  .tSREX          = 35,    /*!< Self Refresh Exit Time */
  .tAPR           = 20,    /*!< Last Data Out to Active Time */
  .tDAL           = 20,    /*!< Data In to Active Command Time */
  .tWR            = 20,    /*!< Write Recovery Time */
  .tRC            = 33,    /*!< Active to Active Command Period */
  .tRFC           = 35,    /*!< Auto-refresh Period */
  .tXSR           = 35,    /*!< Exit Selt Refresh */
  .tRRD           = 12,    /*!< Active Bank A to Active Bank B Time */
  .tMRD           = 2,    /*!< Load Mode register command to Active Command */
  .DevConfig =
  {
    0xA0000000,     /*!< Base Address */
    0x02,      /*!< RAS value */
    LPC178X_EMC_MODEREG_VALUE,  /*!< Mode Register value */
    (LPC178X_EMC_AM << LPC178X_EMC_DYCFG_AM_BITS)
  }
};

/******************************************/
void Sram_Init()
{
  uint32_t index=0;
  LPC_IOCON_T base={0};

  for(index=0; index<(sizeof(pin)/sizeof(PINMUX_GRP_T)); index++)
  {
    base.p[pin[index].pingrp][pin[index].pinnum] = (LPC_IOCON_BASE + (pin[index].pingrp) * 0x80 /
                                                                   + (pin[index].pinnum) * 4);
  }

  Chip_IOCON_SetPinMuxing(&base, pin, sizeof(pin) / sizeof(PINMUX_GRP_T));

  /* Enable peripheral clock */
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);

  /** Clock delay for EMC */
  LPC_SYSCTL->EMCDLYCTL = (LPC178X_EMC_CMDDLY << SCC_EMCDLYCTL_CMDDLY_BITS) |
                          (LPC178X_EMC_FBCLKDLY << SCC_EMCDLYCTL_FBCLKDLY_BITS);

  Chip_EMC_Dynamic_Init(&Sram_Default_Config,1);

}
