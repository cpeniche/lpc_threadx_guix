
export CROSS_COMPILE ?= arm-none-eabi-
export CC := $(CROSS_COMPILE)gcc	
export CPP := $(CROSS_COMPILE)g++	
export CPU := cortex_m3
export APPDIR := application
TOOL := gnu
AR := $(CROSS_COMPILE)ar
export CFLAGS := -mcpu=cortex-m3 -march=armv7-m -mthumb -O0 \
		   -fmessage-length=0 -fsigned-char -ffunction-sections \
		   -fdata-sections -g3 -DCORE_M3 -D__NO_SYSTEM_INIT
export arch_cpu := ports/$(CPU)/$(TOOL)

#Extract threadx directory
export AZURE_DIR ?= /home/carlo/projects/Azure
THREADX_DIR = threadx
GUIX_DIR = guix
USBX_DIR = usbx
LPC_DIR = lpc_chip_177x_8x
FILEX_DIR = filex
export PROGDIR = $(CURDIR)
export BUILDDIR = $(CURDIR)/build
export LIBDIR ?= $(CURDIR)/build
export OBJDIR ?= $(LIBDIR)/obj

#MAKEFLAGS += -rR

INCLUDES += -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc
INCLUDES += -I$(CURDIR)/$(LPC_DIR)/inc

VPATH += $(OBJDIR)/$(THREADX_DIR)
VPATH += $(OBJDIR)/$(GUIX_DIR)
VPATH += $(OBJDIR)/$(LPC_DIR)

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

export Q
export VERBOSE
export INCLUDES

ifeq ($(findstring liblpc,$(MAKECMDGOALS)),liblpc)
include sources.mk
VPATH += $(CURDIR)/$(LPC_DIR)/src
$(shell mkdir -p $(OBJDIR)/$(LPC_DIR))
endif

#$(info $(VPATH))

app: 
	$(Q)$(MAKE) -C  ./$(APPDIR)

######  Compile GUIX Library ############

libgx : 
	$(Q)$(MAKE) -C ./lib/Azure/guix

######  Compile THREADX Library #######

libtx: 
	$(Q)$(MAKE) -C ./lib/Azure/threadx	

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
	$(Q)rm -rf $(LIBDIR)

#remove the build directories and the threadx source links
commit :
	$(Q)rm -rf $(LIBDIR)
	$(Q)$(PROGDIR)/scripts/create_links.sh del $(CURDIR)/lib/Azure/threadx
	$(Q)$(PROGDIR)/scripts/create_links.sh del $(CURDIR)/lib/Azure/guix

src_tree :
	$(Q)$(PROGDIR)/scripts/create_links.sh create $(CURDIR)/lib/Azure/guix