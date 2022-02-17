#include <stdio.h>
#include "lpc_types.h"
#include "chip.h"
#include "tx_api.h"
#include "chip_lpc177x_8x.h"
#include "touch_driver.h"
#include "ssp_17xx_40xx.h"
#include "main.h"

#define SSPx LPC_SSP0
#define CS_PIN 23
#define PORT LPC_GPIO2
#define CS_EN PORT->CLR |= (1 << CS_PIN);
#define CS_DIS PORT->SET |= (1 << CS_PIN);

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

    /* Configure Chip Select */
    PORT->DIR |= 1 << CS_PIN;
    CS_DIS;

    /* Initialize Peripheral */
    Chip_GPIOINT_Init(LPC_GPIOINT);

    /*Configure port for interrupt falling edge */
    Chip_GPIOINT_SetIntFalling(LPC_GPIOINT,GPIOINT_PORT2,1<<11);

    /* Enable interrupt */
    NVIC->ISER[1] |= 1<< 6;

    /* Clear pending interrupt*/
    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT,GPIOINT_PORT2,1<<11);
    
}

void Touch_Screen::Init()
{

    /* Enable Peripheral Clock*/
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SSP0);

    /* Set clock div to zero and prescaler to 27 for a 2 MHZ clock */
    Chip_SSP_SetClockRate(SSPx, 0, 27);

    /* Set sspx to master mode */
    Chip_SSP_SetMaster(SSPx, 1);

    /* Set sspx clock polarity, bits per frame and format*/
    Chip_SSP_SetFormat(SSPx, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);

    /* Enable Peripheral */
    Chip_SSP_Enable(SSPx);
    
}

void Touch_Screen::get_pos(char coordinate, uint8_t idx)
{
    uint32_t i;

    rx_buff[0] = 0x0;
    rx_buff[1] = 0x0;
    CS_EN
    Chip_SSP_WriteFrames_Blocking(SSPx, &tx_buff[coordinate], 1);
    for (i = 0; i < 35000; i++);
    Chip_SSP_ReadFrames_Blocking(SSPx, &rx_buff[0], 2);
    pos[coordinate][idx] = (rx_buff[0] << 4) + (rx_buff[1] >> 4);
    
}

void Touch_Screen::power_down()
{
    CS_DIS

    /* Cleare pending interrupt*/
    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT,GPIOINT_PORT2,1<<11);
}

#ifdef __cplusplus
extern "C" {
#endif

void GPIO_IRQHandler()
{
    UINT status;
    
    /* Cleare pending interrupt*/
    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT,GPIOINT_PORT2,1<<11);

    /* Unblock touch thread */
    Tdrv.int_flag = 1;
}

#ifdef __cplusplus
}
#endif