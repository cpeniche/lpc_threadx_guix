#ifndef  __TOUCH_DRIVER__
#define  __TOUCH_DRIVER__

class Touch_Screen
{
    private:
        const PINMUX_GRP_T pin[4] = {
        {.pingrp = 2, .pinnum = 22, .modefunc = IOCON_FUNC2},   //CLK
        {.pingrp = 2, .pinnum = 23, .modefunc = IOCON_FUNC2},    //CS
        {.pingrp = 2, .pinnum = 26, .modefunc = IOCON_FUNC2},    //MISO
        {.pingrp = 2, .pinnum = 27, .modefunc = IOCON_FUNC2},    //M0SI
        };
        uint8_t tx_buff[20];
        uint8_t rx_buff[20];
    public:
        void Init();
        void IO_config();
};

#endif