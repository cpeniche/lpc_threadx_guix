
CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc	
CPU := cortex_m3
TOOL := gnu
AR := $(CROSS_COMPILE)ar
CFLAGS := -mcpu=cortex-m3 -march=armv7-m -mthumb -O0 \
		   -fmessage-length=0 -fsigned-char -ffunction-sections \
		   -fdata-sections -g3 -std=gnu11	  
arch_cpu := ports/$(CPU)/$(TOOL)

#Extract threadx directory
AZURE_DIR = /home/carlo/projects/Azure
THREADX_DIR = threadx
GUIX_DIR = guix
USBX_DIR = usbx
FILEX_DIR = filex
COMPILE_GX ?=
COMPILE_TX ?=
Q = @
LIBDIR ?= $(CURDIR)/build
OBJDIR ?= $(LIBDIR)/obj

.SUFFIXES:

INCLUDES = -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc

VPATH += $(OBJDIR)/$(THREADX_DIR)
VPATH += $(OBJDIR)/$(GUIX_DIR)

ifeq ("$(origin V)", "command line")
  VERBOSE = $(V)
endif
ifndef VERBOSE
  VERBOSE = 0
endif

ifeq ($(VERBOSE),1)
  Q =
else
  Q = @
endif

ifeq ($(MAKECMDGOALS),libtx)
$(info Entering libtx)
include sources.mk
include libtx_port_files.mk
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/common/src
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/$(arch_cpu)/src
$(shell mkdir -p $(OBJDIR)/$(THREADX_DIR))
endif

ifeq ($(MAKECMDGOALS),libgx)
include sources.mk
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/common/src
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/$(arch_cpu)/src
VPATH += $(HOME)/projects/Azure/$(GUIX_DIR)/common/src
$(shell mkdir -p $(OBJDIR)/$(GUIX_DIR))
endif

######  Compile GUIX Library ############

libgx : $(LIBDIR)/libgx.a

$(LIBDIR)/libgx.a : $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx))
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $?
	@echo 'Finished building target: $@'
	@echo ' '

#common Guix files
$(OBJDIR)/$(GUIX_DIR)/%.o : %.c
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $@

-include $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx:%.o=%.d))


$(OBJDIR)/$(GUIX_DIR)/%.d : %.c
	@echo "building dependency file : $@"
	$(Q)$(CC) -MM -MT$@ -MT$(@:%.d=%.o) $(CFLAGS) $(INCLUDES) $< > $@


######  Compile THREADX Library #######

libtx : $(LIBDIR)/libtx.a

$(LIBDIR)/libtx.a : $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx)) $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx))
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $? 
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
$(OBJDIR)/$(THREADX_DIR)/%.o : %.c
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $@

#port asm files
$(OBJDIR)/$(THREADX_DIR)/%.o : %.S 
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $@

-include $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx:%.o=%.d))

$(OBJDIR)/$(THREADX_DIR)/%.d: %.c
	@echo "building dependency file : $@"
	$(Q)$(CC) -MM -MT$@ -MT$(@:%.d=%.o) $(CFLAGS) $(INCLUDES) $< > $@ \

-include $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx:%.o=%.d))

$(OBJDIR)/$(THREADX_DIR)/%.d: %.S
	@echo "building dependency file : $@"
	$(Q)$(CC) -MM -MT$@ -MT$(@:%.d=%.o) $(CFLAGS) $(INCLUDES) $< > $@ \

	
clean:	
	rm -rf $(LIBDIR)