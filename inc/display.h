#ifndef __DISPLAY_H_
#define __DISPLAY_H_


extern LCD_CONFIG_T lcd_config;
extern uint32_t display_fb[800][480];

void Display_Init_Pinmux();

#endif
