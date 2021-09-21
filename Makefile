
export CROSS_COMPILE ?= arm-none-eabi-
export CC := $(CROSS_COMPILE)gcc	
export CPP := $(CROSS_COMPILE)g++	
export CPU := cortex_m3
TOOL := gnu
AR := $(CROSS_COMPILE)ar
export CFLAGS := -mcpu=cortex-m3 -march=armv7-m -mthumb -O0 \
		   -fmessage-length=0 -fsigned-char -ffunction-sections \
		   -fdata-sections -g3 -DCORE_M3
export arch_cpu := ports/$(CPU)/$(TOOL)

#Extract threadx directory
AZURE_DIR = /home/carlo/projects/Azure
THREADX_DIR = threadx
GUIX_DIR = guix
USBX_DIR = usbx
LPC_DIR = lpc_chip_177x_8x
FILEX_DIR = filex
COMPILE_GX ?=
COMPILE_TX ?=
export Q = @
export LIBDIR ?= $(CURDIR)/build
export OBJDIR ?= $(LIBDIR)/obj

MAKEFLAGS += -rR

INCLUDES =  -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(CURDIR)/$(LPC_DIR)/inc

VPATH += $(OBJDIR)/$(THREADX_DIR)
VPATH += $(OBJDIR)/$(GUIX_DIR)
VPATH += $(OBJDIR)/$(LPC_DIR)

export INCLUDES

-include defines.mk

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

export VERBOSE

ifeq ($(findstring libtx,$(MAKECMDGOALS)),libtx)
include sources.mk
include libtx_port_files.mk
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/common/src
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/$(arch_cpu)/src
$(shell mkdir -p $(OBJDIR)/$(THREADX_DIR))
endif

ifeq ($(findstring libgx,$(MAKECMDGOALS)),libgx)
include sources.mk
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/common/src
VPATH += $(HOME)/projects/Azure/$(THREADX_DIR)/$(arch_cpu)/src
VPATH += $(HOME)/projects/Azure/$(GUIX_DIR)/common/src
$(shell mkdir -p $(OBJDIR)/$(GUIX_DIR))
endif


ifeq ($(findstring liblpc,$(MAKECMDGOALS)),liblpc)
include sources.mk
VPATH += $(CURDIR)/$(LPC_DIR)/src
$(shell mkdir -p $(OBJDIR)/$(LPC_DIR))
endif

app: 
	$(Q)$(MAKE) -C  ./Application

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
	$(Q)$(call compile,$(CC),$<,$@)
	
-include $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx:%.o=%.d))

$(OBJDIR)/$(GUIX_DIR)/%.d : %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))


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
	$(Q)$(call compile,$(CC),$<,$@)

#port asm files
$(OBJDIR)/$(THREADX_DIR)/%.o : %.S 
	$(Q)$(call compile,$(CC),$<,$@)

-include $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx:%.o=%.d))

$(OBJDIR)/$(THREADX_DIR)/%.d: %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))


-include $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx:%.o=%.d))

$(OBJDIR)/$(THREADX_DIR)/%.d: %.S
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))


####-------------------------------------###

liblpc: $(LIBDIR)/liblpc.a

$(LIBDIR)/liblpc.a : $(addprefix $(OBJDIR)/$(LPC_DIR)/,$(obj-lpc)) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $? 
	@echo 'Finished building target: $@'
	@echo ' '

#common lpc files
$(OBJDIR)/$(LPC_DIR)/%.o : %.c
	$(Q)$(call compile,$(CC),$<,$@)

-include $(addprefix $(OBJDIR)/$(LPC_DIR)/,$(obj-lpc:%.o=%.d))

$(OBJDIR)/$(LPC_DIR)/%.d: %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))
	
.phony: libtx libgx liblpc

clean:	
	rm -rf $(LIBDIR)