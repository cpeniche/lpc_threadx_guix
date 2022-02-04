#include "lpc_types.h"
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
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

}

static void display_driver_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area)
{
  memset(display_fb, (int)canvas->gx_canvas_memory, sizeof(display_fb));
}


UINT display_driver_setup(GX_DISPLAY *display)
{
  memset(display_fb,0x0,sizeof(display_fb));

  _gx_display_driver_565rgb_setup(display,GX_NULL,display_driver_toggle);

  return GX_SUCCESS;
}
