/*
 * FLASH_MEMORY.cpp
 *
 *  Created on: Feb 28, 2024
 *      Author: carlo
 */

#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "emc_17xx_40xx.h"
#include "flash_memory.h"
#include "tx_api.h"


void FLASH_MEMORY::IO_Config()
{
	uint32_t index = 0, port = 0, base_addr;
	LPC_IOCON_T base = {0};

	/* Configure pins as table pin[] */
	for (index = 0; index < (sizeof(sram_pin) / sizeof(PINMUX_GRP_T)); index++)
	{
	base.p[sram_pin[index].pingrp][sram_pin[index].pinnum] = (LPC_IOCON_BASE + (sram_pin[index].pingrp) * 0x80 + (sram_pin[index].pinnum) * 4);
	}

	Chip_IOCON_SetPinMuxing(&base, sram_pin, sizeof(sram_pin) / sizeof(PINMUX_GRP_T));

	/* Configure port 3 as EMC data pins*/
	port = 3;
	for (index = 0; index <= LPC178X_SRAM_DATA_PINS; index++)
	{
		base.p[port][index] = (LPC_IOCON_BASE + (port)*0x80 + (index)*4);
		Chip_IOCON_PinMuxSet(&base, port, index, LPC178X_GPIO_EMC_REGVAL);
	}

	/* Configure port 4 as EMC address pins */
	port = 4;
	for (index = 1; index < LPC178X_SRAM_ADDR_PINS+1; index++)
	{
		base.p[port][index] = (LPC_IOCON_BASE + (port)*0x80 + (index)*4);
		Chip_IOCON_PinMuxSet(&base, port, index, LPC178X_GPIO_EMC_REGVAL);
	}
}

void FLASH_MEMORY::Init()
{
	Chip_EMC_Init(1,EMC_CONFIG_ENDIAN_LITTLE);

	/* Enable peripheral clock for EMC*/
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);

	Chip_EMC_Static_Init(FLASH_MEMORY::get_flash_config());

	Chip_SYSCTL_SetFLASHAccess(FLASHTIM_SAFE_SETTING);

	Chip_EMC_AddressChip(FALSE);
}


IP_EMC_STATIC_CONFIG_T *FLASH_MEMORY::get_flash_config()
{
	return &Flash_Config;

}

