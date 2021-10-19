
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
 * IS42S32800B SDRAM: 32-bit, 4 banks, 12 rows, 9 cols.
 * See Table 133 "Address mapping" in the LPC178x/7x User Manual.
 */
#define LPC178X_EMC_AM      0x89


IP_EMC_DYN_CONFIG_T Sram_Default_Config =
{
  .RefreshPeriod    = 1,                   /*!< Refresh period */
  .ReadConfig       = 1                    /*!< Clock*/
  .tRP;                                    /*!< Precharge Command Period */
  .tRAS;                                   /*!< Active to Precharge Command Period */
  .tSREX;                                  /*!< Self Refresh Exit Time */
  .tAPR;                                   /*!< Last Data Out to Active Time */
  .tDAL;                                   /*!< Data In to Active Command Time */
  .tWR;                                    /*!< Write Recovery Time */
  .tRC;                                    /*!< Active to Active Command Period */
  .tRFC;                                   /*!< Auto-refresh Period */
  .tXSR;                                   /*!< Exit Selt Refresh */
  .tRRD;                                   /*!< Active Bank A to Active Bank B Time */
  .tMRD;                                   /*!< Load Mode register command to Active Command */
  {
    .BaseAddr;       /*!< Base Address */
    .RAS;            /*!< RAS value */
    .ModeRegister;   /*!< Mode Register value */
    .DynConfig = (LPC178X_EMC_AM << LPC178X_EMC_DYCFG_AM_BITS);
  }
  };



void sram_init()
{

  /* Enable peripheral clock */
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);

  /** Clock delay for EMC */
  LPC_SYSCTL->EMCDLYCTL = (LPC178X_EMC_CMDDLY << SCC_EMCDLYCTL_CMDDLY_BITS) |
                          (LPC178X_EMC_FBCLKDLY << SCC_EMCDLYCTL_FBCLKDLY_BITS);

  Chip_EMC_Dynamic_Init()

}
