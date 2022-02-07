#include "lpc_types.h"
#include "tx_api.h"
#include "gx_api.h"

#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "iocon_17xx_40xx.h"
#include "lcd_17xx_40xx.h"

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */

#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

#include "gx_display.h"
/* Determine if a C++ compiler is being used.  If so, complete the standard
   C conditional started above.  */
#ifdef __cplusplus
}
#endif

#include "display.h"


uint16_t display_fb[272][480] __attribute__((section(".sram")));
static void display_driver_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area);

Display::Display(/* args */)
{
}

Display::~Display()
{
}

void Display::IO_config()
{
  uint32_t index = 0, port = 0, base_addr;
  LPC_IOCON_T base = {0};
  
  /* Configure pins as table pin[] */
  for (index = 0; index < (sizeof(pin) / sizeof(PINMUX_GRP_T)); index++)
  {
    base.p[pin[index].pingrp][pin[index].pinnum] = (LPC_IOCON_BASE + (pin[index].pingrp) * 0x80 + (pin[index].pinnum) * 4);
  }

  /* configure lcd I/O pins*/
  Chip_IOCON_SetPinMuxing(&base, pin, sizeof(pin) / sizeof(PINMUX_GRP_T));

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

  /* Enable led backlight */
  LPC_GPIO2->DIR |= 0x2;
	LPC_GPIO2->SET |= 0x2;

  /* Enable lcd controller */
  LPC_LCD->CTRL |= CLCDC_LCDCTRL_ENABLE;

}

static void display_driver_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area)
{
  
  /* Copy GUIX local canvas buffer to the lcd buffer*/
  memcpy(display_fb,canvas->gx_canvas_memory,sizeof(display_fb));
}

UINT display_driver_setup(GX_DISPLAY *display)
{
  /* Initialize screen to black */
  memset(display_fb,0xFFFF,sizeof(display_fb));

  /* Initialize driver to use 565 rgb pixel maps*/
  _gx_display_driver_565rgb_setup(display,GX_NULL,display_driver_toggle);

  return GX_SUCCESS;
}