#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#define RGB565(r,g,b) ((b<<11) | ((g&0x2F)<<5) | (r&0x1F))
#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 272

extern UINT display_driver_setup(GX_DISPLAY *display);

class Display
{
    private:
        
        const PINMUX_GRP_T pin[21] = {
        {.pingrp = 2, .pinnum = 12, .modefunc = IOCON_FUNC5},   //R0
        {.pingrp = 2, .pinnum = 6, .modefunc = IOCON_FUNC7},    //R1
        {.pingrp = 2, .pinnum = 7, .modefunc = IOCON_FUNC7},    //R2
        {.pingrp = 2, .pinnum = 8, .modefunc = IOCON_FUNC7},    //R3
        {.pingrp = 2, .pinnum = 9, .modefunc = IOCON_FUNC7},    //R4

        {.pingrp = 1, .pinnum = 20, .modefunc = IOCON_FUNC7},   //G0
        {.pingrp = 1, .pinnum = 21, .modefunc = IOCON_FUNC7},   //G1
        {.pingrp = 1, .pinnum = 22, .modefunc = IOCON_FUNC7},   //G2
        {.pingrp = 1, .pinnum = 23, .modefunc = IOCON_FUNC7},   //G3
        {.pingrp = 1, .pinnum = 24, .modefunc = IOCON_FUNC7},   //G4
        {.pingrp = 1, .pinnum = 25, .modefunc = IOCON_FUNC7},   //G5

        {.pingrp = 2, .pinnum = 13, .modefunc = IOCON_FUNC7},   //B0
        {.pingrp = 1, .pinnum = 26, .modefunc = IOCON_FUNC7},   //B1
        {.pingrp = 1, .pinnum = 27, .modefunc = IOCON_FUNC7},   //B2
        {.pingrp = 1, .pinnum = 28, .modefunc = IOCON_FUNC7},   //B3
        {.pingrp = 1, .pinnum = 29, .modefunc = IOCON_FUNC7},   //B4

        {.pingrp = 2, .pinnum = 0, .modefunc = IOCON_FUNC7},    //DISP        
        {.pingrp = 2, .pinnum = 2, .modefunc = IOCON_FUNC7},    //DCLK
        {.pingrp = 2, .pinnum = 3, .modefunc = IOCON_FUNC7},    //VSYNC
        {.pingrp = 2, .pinnum = 4, .modefunc = IOCON_FUNC7},    //DEN
        {.pingrp = 2, .pinnum = 5, .modefunc = IOCON_FUNC7},    //HSYNC
        };

        LCD_CONFIG_T lcd_config =
        {
        .HBP = 2,                            /*!< Horizontal back porch in clocks */
        .HFP = 2,                            /*!< Horizontal front porch in clocks */
        .HSW = 41,                            /*!< HSYNC pulse width in clocks */
        .PPL = DISPLAY_WIDTH,                /*!< Pixels per line */
        .VBP = 2,                            /*!< Vertical back porch in lines */
        .VFP = 2,                            /*!< Vertical front porch in lines */
        .VSW = 10,                           /*!< VSYNC pulse width in lines */
        .LPP = DISPLAY_HEIGHT,               /*!< Lines per panel */
        .IOE = 0,                             /*!< Invert output enable, 1 = invert */
        .IPC = 1,                             /*!< Invert panel clock, 1 = invert */
        .IHS = 1,                             /*!< Invert HSYNC, 1 = invert */
        .IVS = 1,                             /*!< Invert VSYNC, 1 = invert */
        .ACB = 0,                             /*!< AC bias frequency in clocks (not used) */
        .BPP = 6,                             /*!< Maximum bits per pixel the display supports */
        .LCD = LCD_TFT,                       /*!< LCD panel type */
        .color_format = LCD_COLOR_FORMAT_RGB, /*!<BGR or RGB */
        .Dual = 0
        };
    public:
        
        Display(/* args */);
        ~Display();
        void IO_Config();
        void Init();
};

#endif
