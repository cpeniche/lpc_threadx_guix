#include "chip.h"
#include "tx_api.h"
#include "display.h"

#define DEMO_STACK_SIZE          (2 * 1024)
void  main_thread_entry(ULONG arg);

/* Define global data structures.   */

TX_THREAD  main_thread;
ULONG      class_driver_index;
UINT       status;
CHAR       main_thread_name[]="main thread";


int main()
{

	Chip_SystemInit();

	/*Initialize LCD pin out*/
	Display_Init_Pinmux();

  /* Initialize the LCD controller */
  Chip_LCD_Init(LPC_LCD,&lcd_config);

	LPC_LCD->UPBASE = (uint32_t)display_fb;

  /* Enter the ThreadX kernel. */
  tx_kernel_enter( );
}

void tx_application_define(void *first_unused_memory)
{
	CHAR *stack_pointer;
	CHAR *memory_pointer;

  /* Initialize the free memory pointer */
  stack_pointer =   (CHAR *) first_unused_memory;
  memory_pointer =  stack_pointer + DEMO_STACK_SIZE;

  /* Create the main demo thread.  */
  tx_thread_create(&main_thread, main_thread_name, main_thread_entry, 0,
          stack_pointer, DEMO_STACK_SIZE,
          30, 30, 1, TX_AUTO_START);
}

void  main_thread_entry(ULONG arg)
{

  UINT i=0;

  while(1)
 {
		i++;
 }
}
