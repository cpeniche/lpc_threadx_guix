#ifndef __MAIN_H__
#define __MAIN_H__

typedef struct STRUCT_THREAD_INFO
{
    TX_THREAD *thread;
    char *name;
    void (* entry_func)(ULONG args);
    CHAR *stack_ptr;
    uint32_t stack_size;
    UINT start_type;

}APP_THREAD_INFO;

enum{
    MAIN_THREAD,
    GUI_THREAD,
    TOUCH_THREAD,
};

extern APP_THREAD_INFO thread_list[];
extern Touch_Screen Tdrv;

extern TX_SEMAPHORE touch_semaphore;

#define THREAD_CREATE(id) tx_thread_create(thread_list[id].thread,           /* thread ptr */     \
                                            thread_list[id].name,            /* name   */         \
                                            thread_list[id].entry_func,      /* entry function */ \
                                            0,                               /* entry input */    \
                                            thread_list[id].stack_ptr,       /* stack start */    \
                                            thread_list[id].stack_size,      /* stack size  */    \
                                            30,                              /* priority */       \
                                            30,                              /* preempt threshold */  \
                                            1,                               /* time_slice */         \
                                            thread_list[id].start_type);     /* auto start */



#endif