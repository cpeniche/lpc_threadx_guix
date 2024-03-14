#include "chip.h"
#include "tx_api.h"
#include "sdram.h"
#include "gx_api.h"
#include "gx_display.h"
#include "display.h"
#include "touch_driver.h"
#include "main.h"
#include "flash_memory.h"

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

UINT create_button(GX_WINDOW *widget);

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
SDRAM_Memory SDram;
FLASH_MEMORY ExtFlash;
Display TFT_lcd;

GX_WINDOW_ROOT *root;

int main()
{
	//SDRAM_Memory *test = new SDRAM_Memory();
  //test->Init(tx_thread_sleep);
  Chip_SystemInit();
  SDram.IO_config();
  ExtFlash.IO_Config();
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

  SDram.Init(tx_thread_sleep);
  SDram.Clear(&__bss2_start__, &__bss2_end__);
  ExtFlash.Init();
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

  GX_WIDGET *widget;
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

  //gx_system_widget_find(TEST_WINDOW_ID,GX_SEARCH_DEPTH_INFINITE,&widget);

  //create_button((GX_WINDOW *)widget);

  /* Show the root window to make it visible. */
  gx_widget_show(root);

  /* Let GUIX run. */
  gx_system_start();
}


GX_TEXT_BUTTON button_widget;
GX_STRING btn_text={"text",4};
GX_CONST GX_CHAR *name="text_btn";
GX_STRING string_text = {"¶",strlen(string_text.gx_string_ptr)};


UINT create_button(GX_WINDOW *widget)
{

  GX_RECTANGLE rectangle;
  UINT status = GX_SUCCESS;


  status=gx_utility_rectangle_define(&rectangle,22, 220, 101, 243);

      /* first create the base pixelmap button */
  status=gx_text_button_create(&button_widget,
															 name,
															 widget,
															 0/*GX_STRING_ID_STRING_6*/,
															 GX_STYLE_BORDER_RAISED|GX_STYLE_ENABLED|GX_STYLE_TEXT_CENTER,
															 0,
															 &rectangle);

  if(status != GX_SUCCESS)
	  return status;

  status=gx_widget_fill_color_set((GX_WIDGET *)&button_widget,
		  GX_COLOR_ID_BTN_LOWER,                   /* normal color id                */
		  GX_COLOR_ID_BTN_UPPER,             /* selected color id              */
		  GX_COLOR_ID_DISABLED_FILL);
  if(status != GX_SUCCESS)
	  return status;

  status=gx_text_button_font_set(&button_widget,GX_FONT_ID_BUTTON);

  if(status != GX_SUCCESS)
	  return status;

  status=gx_text_button_text_color_set(&button_widget,GX_COLOR_ID_BTN_TEXT,                    /* normal text color              */
		  GX_COLOR_ID_BTN_TEXT,                    /* selected text color            */
		  GX_COLOR_ID_DISABLED_TEXT);
  if(status != GX_SUCCESS)
	  return status;

  status = gx_text_button_text_set_ext(&button_widget, &string_text);

  if(status != GX_SUCCESS)
	  return status;

  status=gx_widget_status_add((GX_WIDGET *)&button_widget,GX_STATUS_ACCEPTS_FOCUS|GX_STATUS_VISIBLE);

  return status;

}

UINT multiline_handler(GX_MULTI_LINE_TEXT_INPUT *widget, GX_EVENT *event_ptr)
{
	return 1;

}

#ifdef __cplusplus
}
#endif
