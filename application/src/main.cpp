#include "chip.h"
#include "tx_api.h"
#include "sram.h"
#include "gx_api.h"
#include "gx_display.h"
#include "display.h"
#include "sample_resources.h"
#include "sample_specifications.h"

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

  gx_system_initialize();

  gx_studio_display_configure(DISPLAY_1, display_driver_setup, LANGUAGE_ENGLISH, DISPLAY_1_THEME_1,&root);

  gx_studio_named_widget_create((char *)"window",(GX_WIDGET*)root,GX_NULL);

  gx_widget_show(root);

  gx_system_start(); 
}


