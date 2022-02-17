#include "chip.h"
#include "tx_api.h"
#include "sram.h"
#include "gx_api.h"
#include "gx_display.h"
#include "display.h"
#include "touch_driver.h"
#include "main.h"

/* add GUIX generated files*/
#include "3dprint_lcd_resources.h"
#include "3dprint_lcd_specifications.h"


#define STACK_SIZE (2 * 1024)
CHAR main_stack[STACK_SIZE];
CHAR gui_stack[STACK_SIZE]    __attribute__((section(".sram")));
CHAR touch_stack[STACK_SIZE]  __attribute__((section(".sram")));



static void main_thread_entry(ULONG arg);
static void gui_thread_entry(ULONG args);
static void touch_thread_entry(ULONG args);

/* Define global data structures.   */
TX_THREAD main_thread;
TX_THREAD gui_thread;
TX_THREAD touch_thread;

/* semaphore definitions */
TX_SEMAPHORE touch_semaphore;

APP_THREAD_INFO thread_list[]={
  {
   .thread = &main_thread,
   .name = (char *)"main thread",
   .entry_func = main_thread_entry,
   .stack_ptr = main_stack,
   .stack_size = STACK_SIZE,
   .start_type = TX_AUTO_START
   },
   {
   .thread = &gui_thread,
   .name = (char *)"gui thread",
   .entry_func = gui_thread_entry,
   .stack_ptr = gui_stack,
   .stack_size = STACK_SIZE,
   .start_type = TX_DONT_START
   },
   {
   .thread = &touch_thread,
   .name = (char *)"thouch thread",
   .entry_func = touch_thread_entry,
   .stack_ptr = touch_stack,
   .stack_size = STACK_SIZE,
   .start_type = TX_DONT_START
   }
};

ULONG class_driver_index;
UINT status;
CHAR main_thread_name[] = "main thread";
const uint32_t OscRateIn = 12000000;
Memory Sram;
Display TFT_lcd;
Touch_Screen Tdrv;

GX_WINDOW_ROOT *root;

int main()
{

  Chip_SystemInit();
  Sram.IO_config();
  TFT_lcd.IO_config();  
  Tdrv.IO_config();

  /* Enter the ThreadX kernel. */
  tx_kernel_enter();
}

void tx_application_define(void *first_unused_memory)
{
  CHAR *stack_pointer;
  CHAR *memory_pointer;

  /* Create the main demo thread.  */
  THREAD_CREATE(MAIN_THREAD);
}


static void main_thread_entry(ULONG arg)
{

  UINT i = 0;

  Sram.Init(tx_thread_sleep);
  TFT_lcd.Init();
  Tdrv.Init();
  /* Create thread for gui */
  THREAD_CREATE(GUI_THREAD);

  /* Create touch screen thread */
  THREAD_CREATE(TOUCH_THREAD);

  tx_thread_resume(thread_list[GUI_THREAD].thread);
  tx_thread_resume(thread_list[TOUCH_THREAD].thread);
  
  while(true)
  {
    tx_thread_relinquish();
  }
}


static void gui_thread_entry(ULONG args)
{
  /* Initialize the GUIX library */
  gx_system_initialize();

  /* Configure the main display. */
  gx_studio_display_configure(DISPLAY,                      /* Display to configure*/
                              display_driver_setup, /* Driver to use */
                              LANGUAGE_ENGLISH,                  /* Language to install */
                              DISPLAY_THEME_1,        /* Theme to install */
                              &root);                     /* Root window pointer */

  /* Create the screen - attached to root window. */
  gx_studio_named_widget_create((char *)"main_window", (GX_WIDGET *)root, GX_NULL);

  /* Show the root window to make it visible. */
  gx_widget_show(root);

  /* Let GUIX run. */
  gx_system_start();
}

static void touch_thread_entry(ULONG args)
{
  UINT status;
  uint8_t filter_samples=0;
  float filter_data[2], filter_data_1[2]={0};
  static float alpha=0.3, pressure;
  uint16_t rx_plate=805;
  //305

  //tx_semaphore_create(&touch_semaphore,(char *)"touch_semaphore",0);

  while(true)
  {
    
    Tdrv.power_down();

    Tdrv.int_flag=0;

    while(Tdrv.int_flag==0);
    
    /* take n samples of x position*/
    do 
    { 
      /* Read x and y position */
      Tdrv.get_pos(X_POS,0);
      Tdrv.get_pos(Y_POS,0);

      /* apply low pass filter to x position*/
      filter_data[X_POS] = alpha*filter_data_1[X_POS] + (1-alpha)*Tdrv.pos[X_POS][0];
      filter_data_1[X_POS] = filter_data[X_POS];

      /* apply low pass filter to y position*/
      filter_data[Y_POS] = alpha*filter_data_1[Y_POS] + (1-alpha)*Tdrv.pos[Y_POS][0];
      filter_data_1[Y_POS] = filter_data[Y_POS];

      /* Increment sample counts*/
      filter_samples++;
    }while (filter_samples < 5);
    
    filter_samples=0;
    
    /* Calculate pressure */
    //pressure = rx_plate*(filter_data/4096)*((Tdrv.pos[Z2_POS][0]/Tdrv.pos[Z1_POS][0])-1);
    
  }


}
