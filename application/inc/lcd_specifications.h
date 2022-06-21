/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  specification file(s). For more information please refer to the Azure RTOS */
/*  GUIX Studio User Guide, or visit our web site at azure.com/rtos            */
/*                                                                             */
/*  GUIX Studio Revision 6.1.11.0                                              */
/*  Date (dd.mm.yyyy): 20. 6.2022   Time (hh:mm): 21:56                        */
/*******************************************************************************/


#ifndef _LCD_SPECIFICATIONS_H_
#define _LCD_SPECIFICATIONS_H_

#include "gx_api.h"

/* Determine if C++ compiler is being used, if so use standard C.              */
#ifdef __cplusplus
extern   "C" {
#endif

/* Define widget ids                                                           */

#define cal_window_id 1
#define tl_icon_id 2
#define tr_icon_id 3
#define bl_icon_id 4
#define br_icon_id 5
#define done_btn_id 6
#define nozzle_id 7
#define bed_id 8
#define info_btn_id 9


/* Define animation ids                                                        */

#define GX_NEXT_ANIMATION_ID 1


/* Define user event ids                                                       */

enum user_defined_events{
    Ok_Button_Event = GX_FIRST_USER_EVENT,
    GX_NEXT_USER_EVENT_ID
};

#define GX_ACTION_FLAG_DYNAMIC_TARGET 0x01
#define GX_ACTION_FLAG_DYNAMIC_PARENT 0x02
#define GX_ACTION_FLAG_POP_TARGET     0x04
#define GX_ACTION_FLAG_POP_PARENT     0x08

typedef struct GX_STUDIO_ACTION_STRUCT
{
    GX_UBYTE opcode;
    GX_UBYTE flags;
    GX_CONST VOID *parent;
    GX_CONST VOID *target;
    GX_CONST GX_ANIMATION_INFO  *animation;
} GX_STUDIO_ACTION;

typedef struct GX_STUDIO_EVENT_ENTRY_STRUCT
{
    ULONG event_type;
    USHORT event_sender;
    GX_CONST GX_STUDIO_ACTION *action_list;
} GX_STUDIO_EVENT_ENTRY;

typedef struct GX_STUDIO_EVENT_PROCESS_STRUCT 
{
    GX_CONST GX_STUDIO_EVENT_ENTRY *event_table;
    UINT (*chain_event_handler)(GX_WIDGET *, GX_EVENT *);
} GX_STUDIO_EVENT_PROCESS;

/* Declare properties structures for each utilized widget type                 */

typedef struct GX_STUDIO_WIDGET_STRUCT
{
   GX_CHAR *widget_name;
   USHORT  widget_type;
   USHORT  widget_id;
   #if defined(GX_WIDGET_USER_DATA)
   INT   user_data;
   #endif
   ULONG style;
   ULONG status;
   ULONG control_block_size;
   GX_RESOURCE_ID normal_fill_color_id;
   GX_RESOURCE_ID selected_fill_color_id;
   GX_RESOURCE_ID disabled_fill_color_id;
   UINT (*create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   void (*draw_function) (GX_WIDGET *);
   UINT (*event_function) (GX_WIDGET *, GX_EVENT *);
   GX_RECTANGLE size;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *next_widget;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *child_widget;
   ULONG control_block_offset;
   GX_CONST void *properties;
} GX_STUDIO_WIDGET;

typedef struct
{
    GX_CONST GX_STUDIO_WIDGET *widget_information;
    GX_WIDGET        *widget;
} GX_STUDIO_WIDGET_ENTRY;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
    GX_RESOURCE_ID disabled_pixelmap_id;
} GX_PIXELMAP_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
} GX_ICON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
} GX_WINDOW_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID readonly_fill_color_id;
    GX_RESOURCE_ID readonly_text_color_id;
    GX_CHAR *buffer;
    UINT buffer_size;
} GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES;


/* Declare top-level control blocks                                            */

typedef struct CAL_WINDOW_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_ICON Cal_Window_tl_icon;
    GX_ICON Cal_Window_tr_icon;
    GX_ICON Cal_Window_bl_icon;
    GX_ICON Cal_Window_br_icon;
} CAL_WINDOW_CONTROL_BLOCK;

typedef struct INFO_WINDOW_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_PIXELMAP_BUTTON info_window_done_btn;
} INFO_WINDOW_CONTROL_BLOCK;

typedef struct MAIN_WINDOW_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_ICON main_window_nozzle;
    GX_SINGLE_LINE_TEXT_INPUT main_window_nozzle_temp;
    GX_ICON main_window_bed;
    GX_SINGLE_LINE_TEXT_INPUT main_window_bed_temp;
    GX_PIXELMAP_BUTTON main_window_settings_btn;
    GX_PIXELMAP_BUTTON main_window_bed_leveling_btn;
    GX_PIXELMAP_BUTTON main_window_adjust_btn;
    GX_PIXELMAP_BUTTON main_window_info_btn;
} MAIN_WINDOW_CONTROL_BLOCK;


/* extern statically defined control blocks                                    */

#ifndef GUIX_STUDIO_GENERATED_FILE
extern CAL_WINDOW_CONTROL_BLOCK Cal_Window;
extern INFO_WINDOW_CONTROL_BLOCK info_window;
extern MAIN_WINDOW_CONTROL_BLOCK main_window;
#endif

/* Declare event process functions, draw functions, and callback functions     */


/* Declare the GX_STUDIO_DISPLAY_INFO structure                                */


typedef struct GX_STUDIO_DISPLAY_INFO_STRUCT 
{
    GX_CONST GX_CHAR *name;
    GX_CONST GX_CHAR *canvas_name;
    GX_CONST GX_THEME **theme_table;
    GX_CONST GX_STRING **language_table;
    USHORT   theme_table_size;
    USHORT   language_table_size;
    UINT     string_table_size;
    UINT     x_resolution;
    UINT     y_resolution;
    GX_DISPLAY *display;
    GX_CANVAS  *canvas;
    GX_WINDOW_ROOT *root_window;
    GX_COLOR   *canvas_memory;
    ULONG      canvas_memory_size;
    USHORT     rotation_angle;
} GX_STUDIO_DISPLAY_INFO;


/* Declare Studio-generated functions for creating top-level widgets           */

UINT gx_studio_pixelmap_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_icon_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_window_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_text_input_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
GX_WIDGET *gx_studio_widget_create(GX_BYTE *storage, GX_CONST GX_STUDIO_WIDGET *definition, GX_WIDGET *parent);
UINT gx_studio_named_widget_create(char *name, GX_WIDGET *parent, GX_WIDGET **new_widget);
UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *), GX_UBYTE language, USHORT theme, GX_WINDOW_ROOT **return_root);
UINT gx_studio_auto_event_handler(GX_WIDGET *widget, GX_EVENT *event_ptr, GX_CONST GX_STUDIO_EVENT_PROCESS *record);

/* Determine if a C++ compiler is being used.  If so, complete the standard
  C conditional started above.                                                 */
#ifdef __cplusplus
}
#endif

#endif                                       /* sentry                         */
