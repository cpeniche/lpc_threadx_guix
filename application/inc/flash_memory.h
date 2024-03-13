/*
 * FLASH_MEMORY.h
 *
 *  Created on: Feb 28, 2024
 *      Author: carlo
 */

#ifndef _FLASH_MEMORY_H_
#define _FLASH_MEMORY_H_

#include "sdram.h"

#define LPC178X_SRAM_DATA_PINS 16
#define LPC178X_SRAM_ADDR_PINS 22
#define SYSCTL_EMCS        (1 << 0)	/*!< SCS register - Controls how addresses are output to EMC */


class FLASH_MEMORY
{
public:
	 void Init();
	 void IO_Config();
	 FLASH_MEMORY (){};
	 ~FLASH_MEMORY (){};

private:
  /* data */
	const PINMUX_GRP_T pin[4] = {
	/* Configure EMC Output Enable */
	{4, 24, LPC178X_GPIO_EMC_REGVAL},
	/* Configure EMC write enable (WE) */
	{4, 25, LPC178X_GPIO_EMC_REGVAL},
	/* Configure EMC CS1 */
	{4, 31, LPC178X_GPIO_EMC_REGVAL},
	/* Configure EMC column address strobe (CAS) */
	{2, 16, LPC178X_GPIO_EMC_REGVAL},
  };

	IP_EMC_STATIC_CONFIG_T Flash_Config =
	{
	  .ChipSelect=1,
		.Config= EMC_STATIC_CONFIG_MEM_WIDTH_16| EMC_STATIC_CONFIG_BLS_HIGH \
		         |EMC_STATIC_CONFIG_EW_DISABLE,

	};
   IP_EMC_STATIC_CONFIG_T *get_flash_config();
};

#endif /* _FLASH_MEMORY_H_ */
