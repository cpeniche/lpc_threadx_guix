
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
LIBDIR ?= /home/carlo/temp/build
OBJDIR ?= /home/carlo/temp/build/obj

INCLUDES = -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc

obj-tx ?=
asm-tx ?=
obj-gx ?=
asm-gx ?=



VPATH = $(OBJDIR)
VPATH += $(OBJDIR)/threadx

#$(info $(VPATH))

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
include sources.mk
include libtx_port_files.mk
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/common/src
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/$(arch_cpu)/src
$(shell mkdir -p $(OBJDIR)/$(THREADX_DIR))
endif

ifeq ($(MAKECMDGOALS),libgx)
include sources.mk
include libtx_port_files.mk
VPATH += $(HOME)/projects/Azure/$(GUIX_DIR)/common/src
endif

libtx : $(LIBDIR)/libtx.a

######  Compile THREADX Library #######
$(LIBDIR)/libtx.a : $(obj-tx) $(asm-tx)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx)) $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx))
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
$(obj-tx): %.o : %.c
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $(OBJDIR)/$(THREADX_DIR)/$@

#port asm files
$(asm-tx): %.o : %.S 
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $(OBJDIR)/$(THREADX_DIR)/$@

-include $(obj-tx:%.o=%.d)
-include $(asm-tx:%.o=%.d)

%.d: %.c
	$(Q)$(CC) -MM -MF$(OBJDIR)/$(THREADX_DIR)/$@ $(CFLAGS) $(INCLUDES) $<


#sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $(OBJDIR)/$(THREADX_DIR)/$@.$$$$ > $(OBJDIR)/$(THREADX_DIR)/$@; \
#rm -f $(OBJDIR)/$(THREADX_DIR)/$@.$$$$

######  Compile GUIX Library ############

libgx : $(LIBDIR)/libgx.a

$(LIBDIR)/libgx.a : $(obj-gx) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx))
	@echo 'Finished building target: $@'
	@echo ' '

#common Guix files
$(obj-gx): %.o : %.c                         
	@echo "[CC]: $<" 
	$(Q)$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $(OBJDIR)/$(GUIX_DIR)/$@
	
clean:	
	rm -rf $(LIBDIR)