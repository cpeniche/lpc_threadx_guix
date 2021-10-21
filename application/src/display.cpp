#include "lpc_types.h"
#include "tx_api.h"
#include "chip.h"
#include "chip_lpc177x_8x.h"
#include "iocon_17xx_40xx.h"
#include "lcd_17xx_40xx.h"


LPC_IOCON_T pinmux;  /* Hold the address of each IO_CON registers */
uint16_t display_fb[40][24] __attribute__((section(".sram")));
LPC_LCD_T lcd_regs;

const PINMUX_GRP_T pin[] =
{
  {.pingrp = 2, .pinnum = 12, .modefunc = IOCON_FUNC5},
  {.pingrp = 2, .pinnum = 6, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 7, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 8, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 9, .modefunc = IOCON_FUNC7},

  {.pingrp = 1, .pinnum = 20, .modefunc = IOCON_FUNC7},
  {.pingrp = 1, .pinnum = 21, .modefunc = IOCON_FUNC7},
  {.pingrp = 1, .pinnum = 22, .modefunc = IOCON_FUNC7},
  {.pingrp = 1, .pinnum = 23, .modefunc = IOCON_FUNC7},
  {.pingrp = 1, .pinnum = 24, .modefunc = IOCON_FUNC7},
  {.pingrp = 1, .pinnum = 25, .modefunc = IOCON_FUNC7},

  {.pingrp = 2, .pinnum = 13, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 26, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 27, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 28, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 29, .modefunc = IOCON_FUNC7},

  {.pingrp = 2, .pinnum = 0, .modefunc = IOCON_FUNC7},
 // {.pingrp = 2, .pinnum = 1, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 2, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 3, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 4, .modefunc = IOCON_FUNC7},
  {.pingrp = 2, .pinnum = 5, .modefunc = IOCON_FUNC7},

  };

  LCD_CONFIG_T lcd_config =
  {
  .HBP = 88,                            /*!< Horizontal back porch in clocks */
  .HFP = 40,                            /*!< Horizontal front porch in clocks */
  .HSW = 48,                            /*!< HSYNC pulse width in clocks */
  .PPL = 800,                           /*!< Pixels per line */
  .VBP = 32,                            /*!< Vertical back porch in clocks */
  .VFP = 13,                            /*!< Vertical front porch in clocks */
  .VSW = 3,                             /*!< VSYNC pulse width in clocks */
  .LPP = 480,                           /*!< Lines per panel */
  .IOE = 0,                             /*!< Invert output enable, 1 = invert */
  .IPC = 0,                             /*!< Invert panel clock, 1 = invert */
  .IHS = 0,                             /*!< Invert HSYNC, 1 = invert */
  .IVS = 0,                             /*!< Invert VSYNC, 1 = invert */
  .ACB = 0,                             /*!< AC bias frequency in clocks (not used) */
  .BPP = 6,                             /*!< Maximum bits per pixel the display supports */
  .LCD = LCD_TFT,                       /*!< LCD panel type */
  .color_format = LCD_COLOR_FORMAT_RGB, /*!<BGR or RGB */
  .Dual = 0};



/************************************/
void Display_Init_Pinmux()
{

  int i, y;
  ULONG base = LPC_IOCON_BASE;
  uint32_t test = 0;

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


void Initialize_Display(void)
{
/*Initialize LCD pin out*/
	Display_Init_Pinmux();

	/* Initialize the LCD controller */
	Chip_LCD_Init(LPC_LCD,&lcd_config);

	/* load frame buffer address */
	LPC_LCD->UPBASE = (uint32_t)display_fb;
	LPC_LCD->LPBASE = (uint32_t)display_fb;
	
	/* Enable lcd controller */
	LPC_LCD->CTRL |= (1 << 0);
	
	/* Enable LCD Power */
	LPC_LCD->CTRL |= (1 << 11);

	/* Set backlight on*/
	LPC_GPIO2->DIR |= 0x2;
	LPC_GPIO2->SET |= 0x2;

	memset(display_fb,0x5555, sizeof(display_fb));

}
