
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

IP_EMC_DYN_CONFIG_T Sram_Default_Config  =
{
  .RefreshPeriod  = 0x1D,    /*!< Refresh period 7.8us/row */
  .ReadConfig     = 0x01,    /*!< Clock*/
  .tRP            = 0x01,    /*!< Precharge Command Period */
  .tRAS           = 0x03,    /*!< Active to Precharge Command Period */
  .tSREX          = 0x03,    /*!< Self Refresh Exit Time */
  .tAPR           = 0x01,    /*!< Last Data Out to Active Time */
  .tDAL           = 0x01,    /*!< Data In to Active Command Time */
  .tWR            = 0x01,    /*!< Write Recovery Time */
  .tRC            = 0x03,    /*!< Active to Active Command Period */
  .tRFC           = 0x03,    /*!< Auto-refresh Period */
  .tXSR           = 0x03,    /*!< Exit Selt Refresh */
  .tRRD           = 0x00,    /*!< Active Bank A to Active Bank B Time */
  .tMRD           = 0x00,    /*!< Load Mode register command to Active Command */
  .DevConfig =
  {
    0x00,     /*!< Base Address */
    0x02,      /*!< RAS value */
    LPC178X_EMC_MODEREG_VALUE,  /*!< Mode Register value */
    (LPC178X_EMC_AM << LPC178X_EMC_DYCFG_AM_BITS)
  }
};

/******************************************/
void Sram_Init()
{

  /* Enable peripheral clock */
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);

  /** Clock delay for EMC */
  LPC_SYSCTL->EMCDLYCTL = (LPC178X_EMC_CMDDLY << SCC_EMCDLYCTL_CMDDLY_BITS) |
                          (LPC178X_EMC_FBCLKDLY << SCC_EMCDLYCTL_FBCLKDLY_BITS);

  Chip_EMC_Dynamic_Init(&Sram_Default_Config);

}
