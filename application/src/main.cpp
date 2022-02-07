#include "chip.h"
#include "tx_api.h"
#include "sram.h"
#include "gx_api.h"
#include "gx_display.h"
#include "display.h"

/* add GUIX generated files*/
#include "3dprint_lcd_resources.h"
#include "3dprint_lcd_specifications.h"

#define DEMO_STACK_SIZE (2 * 1024)
void main_thread_entry(ULONG arg);

/* Define global data structures.   */

TX_THREAD main_thread;
ULONG class_driver_index;
UINT status;
CHAR main_thread_name[] = "main thread";
const uint32_t OscRateIn = 12000000;
Memory Sram;
Display TFT_lcd;

GX_WINDOW_ROOT *root;

int main()
{

  Chip_SystemInit();

  Sram.IO_config();
  TFT_lcd.IO_Config();  
  /* Enter the ThreadX kernel. */
  tx_kernel_enter();
}

void tx_application_define(void *first_unused_memory)
{
  CHAR *stack_pointer;
  CHAR *memory_pointer;

  /* Initialize the free memory pointer */
  stack_pointer = (CHAR *)first_unused_memory;
  memory_pointer = stack_pointer + DEMO_STACK_SIZE;

  /* Create the main demo thread.  */
  tx_thread_create(&main_thread, main_thread_name, main_thread_entry, 0,
                   stack_pointer, DEMO_STACK_SIZE,
                   30, 30, 1, TX_AUTO_START);             
}


void main_thread_entry(ULONG arg)
{

  UINT i = 0;

  Sram.Init(tx_thread_sleep);
  TFT_lcd.Init();

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


