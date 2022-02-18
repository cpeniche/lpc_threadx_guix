#include <stdio.h>
#include "lpc_types.h"
#include "chip.h"
#include "tx_api.h"
#include "gx_api.h"
#include "chip_lpc177x_8x.h"
#include "touch_driver.h"
#include "ssp_17xx_40xx.h"
#include "main.h"
#include "3dprint_lcd_specifications.h"

#define SSPx LPC_SSP0
#define CS_PIN 23
#define PORT LPC_GPIO2
#define CS_EN PORT->CLR |= (1 << CS_PIN);
#define CS_DIS PORT->SET |= (1 << CS_PIN);

#define OK_BTN_PORT LPC_GPIO


#define EN_INT  NVIC->ISER[1] |= 1<< 6;
#define DIS_INT NVIC->ICER[1] |= 1<< 6;


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

    /*Configure p2.11 for interrupt falling edge */
    Chip_GPIOINT_SetIntFalling(LPC_GPIOINT,GPIOINT_PORT2,1<<11);

    /*Configure p0.21 for interrupt falling edge */
    Chip_GPIOINT_SetIntFalling(LPC_GPIOINT,GPIOINT_PORT0,1<<21);
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

void Touch_Screen::get_pos(char coordinate, uint8_t mask)
{
    uint32_t i;

    tx_buff[coordinate] |= mask;
    rx_buff[0] = 0x0;
    rx_buff[1] = 0x0;
    CS_EN
    Chip_SSP_WriteFrames_Blocking(SSPx, &tx_buff[coordinate], 1);
    for (i = 0; i < 35000; i++);
    Chip_SSP_ReadFrames_Blocking(SSPx, &rx_buff[0], 2);
    pos[coordinate] = (rx_buff[0] << 4) + (rx_buff[1] >> 4);
    
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

TX_THREAD touch_thread;
CHAR touch_stack[STACK_SIZE]  __attribute__((section(".sram")));

void GPIO_IRQHandler()
{
    uint32_t status;
    GX_EVENT event;
    
    status = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT,GPIOINT_PORT2);

    if (status & 1<<11)
    {
        /* Clear pending interrupt for port p2.11*/
        Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT,GPIOINT_PORT2,1<<11);

        /* Unblock touch thread */
        Tdrv.int_flag++;
    }
    else
    {
        /* Clear pending interrupt for port p0.21*/
        Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT,GPIOINT_PORT0,1<<21);
        
        /* Load event type */
        event.gx_event_type=Ok_Button_Event;

        /* Specify which widget to send the event */
        event.gx_event_target = (GX_WIDGET *)&Cal_Window;
                
        /* Send event */        
        gx_system_event_send(&event);
    }
}


void touch_thread_entry(ULONG args)
{
  UINT status;
  uint8_t filter_samples=0, PD0=0x0;
  float filter_data[2], filter_data_1[2]={0};
  static float alpha=0.3, pressure;
  uint16_t rx_plate=805;


  /* Send touch driver to power down */
  Tdrv.get_pos(X_POS,PD0);
  
  while(true)
  {
    
    /* Enable Interrupts */
    EN_INT

    /* reset flag and wait for a touch */
    Tdrv.int_flag=0;

    while(Tdrv.int_flag==0)
    {
        tx_thread_relinquish();
    }

    /* Disable interrupts */
    DIS_INT

    /* Keep the adc on between measurements and avoid
       false interrupts for Pen interrupt pin*/
    PD0 = 0x0;
    
    /* take n samples of x and y position*/
    do 
    { 
      /* Read x and y position */
      Tdrv.get_pos(X_POS,PD0);
      Tdrv.get_pos(Y_POS,PD0);

      /* apply low pass filter to x position*/
      filter_data[X_POS] = alpha*filter_data_1[X_POS] + (1-alpha)*Tdrv.pos[X_POS];
      filter_data_1[X_POS] = filter_data[X_POS];

      /* apply low pass filter to y position*/
      filter_data[Y_POS] = alpha*filter_data_1[Y_POS] + (1-alpha)*Tdrv.pos[Y_POS];
      filter_data_1[Y_POS] = filter_data[Y_POS];

      /* Increment sample counts*/
      filter_samples++;
    }while (filter_samples < 5);

    /* update values after filter applied */
    Tdrv.x_pos=filter_data[X_POS];
    Tdrv.y_pos=filter_data[Y_POS];
    
    /* reset sample count */
    filter_samples=0;
        
  }
}


#ifdef __cplusplus
}
#endif