#include "lpc_types.h"
#include "tx_api.h"
#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "iocon_17xx_40xx.h"
#include "lcd_17xx_40xx.h"
#include "display.h"

uint16_t display_fb[272][480] __attribute__((section(".sram")));

Display::Display(/* args */)
{
}

Display::~Display()
{
}

void Display::IO_Config()
{
  int i, y;
  ULONG base = LPC_IOCON_BASE;
  uint32_t test = 0;
  LPC_IOCON_T pinmux; /* Hold the address of each IO_CON registers */

  /* fill up all the IOCON addresses on the pinmux struct*/
  for (i = 0; i < 5; i++)
  {
    for (y = 0; y < 32; y++)
    {
      pinmux.p[i][y] = base + (y * 4);
    }
    base = pinmux.p[i][y - 1] + 4;
  }

  /* configure lcd I/O pins*/
  Chip_IOCON_SetPinMuxing(&pinmux, pin, sizeof(pin) / sizeof(PINMUX_GRP_T));

}

void Display::Init()
{
  uint16_t i, j = 0;
  uint16_t color = 0;
  uint8_t r,g,b=0;

  /* Initialize the LCD controller */
  Chip_LCD_Init(LPC_LCD, &lcd_config);

  /* load frame buffer address */
  LPC_LCD->UPBASE = (uint32_t)display_fb;
  LPC_LCD->LPBASE = (uint32_t)display_fb;

  /* Enable LCD Power */
  LPC_LCD->CTRL |= CLCDC_LCDCTRL_PWR;

  LPC_GPIO2->DIR |= 0x2;
	LPC_GPIO2->SET |= 0x2;

  /* Enable lcd controller */
  LPC_LCD->CTRL |= CLCDC_LCDCTRL_ENABLE;

  for(j=0;j<272;j++) 
    for(i=0;i<480;i++)
      display_fb[j][i]=RGB565(0x1F,0x0,0x0);;
  
  

}

