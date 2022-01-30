#ifndef __SRAM_H__
#define __SRAM_H__




/* EMC data pins (DQ0..DQ31) */
#define LPC178X_EMC_DATA_PINS 31

/* EMC row/column address pins (A0..A11) */
#define LPC178X_EMC_ADDR_PINS 12
/*
 * EMC Delay Control register
 */
/* Programmable delay value for EMC outputs in command delayed mode */
#define SCC_EMCDLYCTL_CMDDLY_BITS 0
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
#define LPC178X_EMC_CMDDLY 0x10
/*
 * Programmable delay value for the feedback clock that controls input data
 * sampling
 */
#define LPC178X_EMC_FBCLKDLY 0x10
/*
 * K4M513233 SDRAM: 32-bit, 4 banks, 12 rows, 10 cols.
 * See Table 133 "Address mapping" in the LPC178x/7x User Manual.
 */

#define LPC178X_EMC_AM 0x89

/* Address mapping */
#define LPC178X_EMC_DYCFG_AM_BITS 7

/* First Sram Address */
#define SRAM_ADDRESS 0xA0000000

#define LPC178X_EMC_MODEREG_BL 2  /* Burst Length code */
#define LPC178X_EMC_MODEREG_CAS 2 /* CAS Latency */

/*
 * The SDRAM chip (K4M513233) mode register.
 * See K4M513233 datasheet
 */
#define LPC178X_EMC_MODEREG_BL_BITS 0  /* Burst Length */
#define LPC178X_EMC_MODEREG_CAS_BITS 4 /* CAS Latency */

#define LPC178X_EMC_MODEREG_VALUE                            \
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

  /*
 * Timing for 54MHz bus
 */
#define LPC178X_EMC_RAS		2
#define LPC178X_EMC_CAS		2
/* Command delayed strategy, using EMCCLKDELAY */
#define LPC178X_EMC_RDCFG_RD	1
/* Precharge command period (tRP) = 1 EMC clock cycle */
#define LPC178X_EMC_T_RP	1
/* Active to precharge command perion (tRAS) = 3 clocks */
#define LPC178X_EMC_T_RAS	3
/* Self-refresh exit time (tSREX) = 4 clocks */
#define LPC178X_EMC_T_SREX	4
/* Last-data-out to active command time (tAPR) = 2 clocks */
#define LPC178X_EMC_T_APR	2
/* Data-in to active command (tDAL) = 2 clocks */
#define LPC178X_EMC_T_DAL	2
/* Write recovery time (tWR) = 2 clocks */
#define LPC178X_EMC_T_WR	2
/* Active to active command perion (tRC) = 4 clocks */
#define LPC178X_EMC_T_RC	4
/*
 * Auto-refresh period and auto-refresh to active command period
 * (tRFC) = 4 clocks
 */
#define LPC178X_EMC_T_RFC	4
/* Exit self-refresh to active command time (tXSR) = 4 clocks */
#define LPC178X_EMC_T_XSR	4
/* Active bank A to active bank B latency (tRRD) = 1 clock */
#define LPC178X_EMC_T_RRD	1
/* Load mode register to active command time (tMRD) = 1 clock */
#define LPC178X_EMC_T_MRD	1
/*
 * Dynamic Memory Configuration register
 */
/* Address mapping */
#define LPC178X_EMC_DYCFG_AM_BITS	7
/* Buffer enable */
#define LPC178X_EMC_DYCFG_B_MSK		(1 << 19)
/*
 * Dynamic Memory RAS & CAS Delay register
 */
/* RAS latency */
#define LPC178X_EMC_DYRASCAS_RAS_BITS	0
/* CAS latency */
#define LPC178X_EMC_DYRASCAS_CAS_BITS	8

/*
 * Dynamic Memory Read Configuration register:
 *     Read data strategy (RD)
 */
#define LPC178X_EMC_DYRDCFG_RD_BITS	0

#define LPC178X_AHB_PERIPH_BASE		0x20080000
#define LPC178X_EMC_BASE		(LPC178X_AHB_PERIPH_BASE + 0x0001C000)
#define LPC178X_EMC			((volatile struct lpc178x_emc_regs *) \
					LPC178X_EMC_BASE)

/*
 * Dynamic Memory Control register
 */

/* Dynamic memory clock enable (CE) */
#define LPC178X_EMC_DYCTRL_CE_MSK	(1 << 0)
/* Dynamic memory clock control (CS) */
#define LPC178X_EMC_DYCTRL_CS_MSK	(1 << 1)
/* SDRAM initialization (I) */
#define LPC178X_EMC_DYCTRL_I_BITS	7
#define LPC178X_EMC_DYCTRL_I_NORMAL	0
#define LPC178X_EMC_DYCTRL_I_MODE	1
#define LPC178X_EMC_DYCTRL_I_PALL	2	/* precharge all */
#define LPC178X_EMC_DYCTRL_I_NOP	3	/* no operation */


#define LPC178X_EMC_MODEREG_ADDR_SHIFT	13

/*
 * Refresh timer.
 * Indicates the multiple of 16 CCLKs between SDRAM refresh cycles.
 */
#define LPC178X_EMC_REFRESH		0x2E
#define LPC178X_EMC_REFRESH_FAST	2	/* Only for initialization */

#define LPC178X_EMC_CTRL_EN_MSK		(1 << 0)
/*
 * EMC Delay Control register
 */
/* Programmable delay value for EMC outputs in command delayed mode */
#define LPC178X_SCC_EMCDLYCTL_CMDDLY_BITS	0
/*
 * Programmable delay value for the feedback clock that controls input data
 * sampling
 */
#define LPC178X_SCC_EMCDLYCTL_FBCLKDLY_BITS	8

/*
 * SCC registers base
 */
#define LPC178X_APB_PERIPH_BASE		0x40000000
#define LPC178X_APB0PERIPH_BASE		(LPC178X_APB_PERIPH_BASE + 0x00000000)
#define LPC178X_APB1PERIPH_BASE		(LPC178X_APB_PERIPH_BASE + 0x00080000)
#define LPC178X_AHB_PERIPH_BASE		0x20080000
#define LPC178X_SCC_BASE		(LPC178X_APB1PERIPH_BASE + 0x0007C000)




extern void Sram_Init();

#endif