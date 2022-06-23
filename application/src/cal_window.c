#include "gx_api.h"
#include "lcd_specifications.h"
#include "cal_window.h"

UINT Cal_Window_Event_Process(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
  switch(event_ptr->gx_event_type)
  {
    case GX_EVENT_SHOW:
      return gx_window_event_process(widget,event_ptr);

    default: 
      return gx_window_event_process(widget,event_ptr);
  }
  return GX_SUCCESS;
}
