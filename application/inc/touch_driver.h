#ifndef  __TOUCH_DRIVER__
#define  __TOUCH_DRIVER__

#define X_POS 0
#define Y_POS 1
#define Z1_POS 2
#define Z2_POS 3
#define POWER_DOWN 4

class Touch_Screen
{
    private:
        const PINMUX_GRP_T pin[4] = {
       
        {.pingrp = 2, .pinnum = 22, .modefunc = IOCON_FUNC2},   //CLK        
        {.pingrp = 2, .pinnum = 26, .modefunc = IOCON_FUNC2},    //MISO
        {.pingrp = 2, .pinnum = 27, .modefunc = IOCON_FUNC2},    //M0SI       
        };
        /* Reference is off, differential mode reading */
        uint8_t tx_buff[4]={0xD0,0x90,0xB0,0xC0};
        uint8_t rx_buff[2];
        Chip_SSP_DATA_SETUP_T xfer;
    public:
        uint16_t pos[4]={0x0};
        volatile uint8_t int_flag;
        void Init();
        void IO_config();
        void get_pos(char coordinate, uint8_t mask);
        void power_down();
        uint16_t x_pos;
        uint16_t y_pos;
        
};

#ifdef __cplusplus
extern "C" {
#endif

#define STACK_SIZE (2 * 1024)

extern TX_THREAD touch_thread;
extern CHAR touch_stack[STACK_SIZE];
extern void touch_thread_entry(ULONG args);


#ifdef __cplusplus
}
#endif

#endif