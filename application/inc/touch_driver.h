#ifndef  __TOUCH_DRIVER__
#define  __TOUCH_DRIVER__

#define X_POS 0
#define Y_POS 1

class Touch_Screen
{
    private:
        const PINMUX_GRP_T pin[4] = {
            /*
        {.pingrp = 2, .pinnum = 22, .modefunc = IOCON_FUNC2},   //CLK
        {.pingrp = 2, .pinnum = 23, .modefunc = IOCON_FUNC2},    //CS
        {.pingrp = 2, .pinnum = 26, .modefunc = IOCON_FUNC2},    //MISO
        {.pingrp = 2, .pinnum = 27, .modefunc = IOCON_FUNC2},    //M0SI*/
        {.pingrp = 0, .pinnum = 15, .modefunc = IOCON_FUNC2},   //CLK
        {.pingrp = 0, .pinnum = 16, .modefunc = IOCON_FUNC2},    //CS
        {.pingrp = 0, .pinnum = 17, .modefunc = IOCON_FUNC2},    //MISO
        {.pingrp = 0, .pinnum = 18, .modefunc = IOCON_FUNC2},    //M0SI*/
        };
        uint8_t tx_buff;
        uint8_t rx_buff[2];
        Chip_SSP_DATA_SETUP_T xfer;
    public:
        uint16_t x_pos;
        uint16_t y_pos;
        void Init();
        void IO_config();
        void get_pos(char coordinate);
        
};

#endif