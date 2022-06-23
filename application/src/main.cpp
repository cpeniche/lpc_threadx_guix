#include "chip.h"
#include "tx_api.h"
#include "sram.h"
#include "gx_api.h"
#include "gx_display.h"
#include "display.h"
#include "touch_driver.h"
#include "main.h"

/* add GUIX generated files*/
#include "lcd_resources.h"
#include "lcd_specifications.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ULONG __bss2_start__;
extern ULONG __bss2_end__;

#define STACK_SIZE (2 * 1024)
CHAR main_stack[STACK_SIZE];
CHAR gui_stack[STACK_SIZE] __attribute__((section(".sram")));

static void main_thread_entry(ULONG arg);
static void gui_thread_entry(ULONG args);

/* Define global data structures.   */
TX_THREAD main_thread;
TX_THREAD gui_thread;

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

GX_WINDOW_ROOT *root;

int main()
{
  Memory *test = new Memory();
  test->Init(tx_thread_sleep);
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
  Sram.Clear(&__bss2_start__, &__bss2_end__);
  TFT_lcd.Init();
  Tdrv.Init(gx_system_event_send);
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

  gx_studio_named_widget_create((char *)"Cal_Window", (GX_WIDGET *)root, GX_NULL);
  
  gx_studio_named_widget_create((char *)"info_window", (GX_WIDGET *)root, GX_NULL);

  /* Create the screen - attached to root window. */
  gx_studio_named_widget_create((char *)"main_window", (GX_WIDGET *)root, GX_NULL);

  /* Show the root window to make it visible. */
  gx_widget_show(root);

  /* Let GUIX run. */
  gx_system_start();
}

#ifdef __cplusplus
}
#endif
