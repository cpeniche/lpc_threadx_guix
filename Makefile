export CROSS_COMPILE ?= arm-none-eabi-
export CC := $(CROSS_COMPILE)gcc	
export CPP := $(CROSS_COMPILE)g++	
export ARCH ?= cortex_m3
export APPDIR := application
export TOOL ?= gnu
export OBJCOPY := $(CROSS_COMPILE)objcopy


export AR := $(CROSS_COMPILE)ar
export CFLAGS := -mcpu=cortex-m3 -march=armv7-m -mthumb -O0 \
		   -fmessage-length=0 -fsigned-char -ffunction-sections \
		   -fdata-sections -g3 -DCORE_M3 -D__NO_SYSTEM_INIT -D__STARTUP_CLEAR_BSS_MULTIPLE

LDFLAGS = -mcpu=cortex-m3  \
		--specs=nosys.specs -Wl,-Map="app.map" -Wl,--gc-sections -static \
		--specs=nano.specs

SHELL:=/bin/bash

#Extract threadx directory
export AZURE_DIR ?= /home/carlo/projects/Azure
THREADX_DIR = threadx
GUIX_DIR = guix
USBX_DIR = usbx
export LPC_DIR = lpc_chip_177x_8x
FILEX_DIR = filex

BUILD_BUILTIN := 1

export BUILD_BUILTIN
export PROGDIR = $(CURDIR)
export BUILDDIR = $(CURDIR)/build
export LIBDIR ?= $(CURDIR)/build
export OBJDIR ?= $(LIBDIR)/obj

srctree := .
objtree := .
VPATH := $(src_tree)

export srctree objtree VPATH

MAKEFLAGS += -rR --no-print-directory

include ./scripts/Makefile.include

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

export cmd_file := application/gcc_arm.ld
_dirs := application/ lib/Azure/threadx/ lib/Azure/guix/ lib/lpc_chip_177x_8x/

built-ins := $(patsubst %/,%/built-in.a, $(_dirs))

PHONY = $(built-ins) FORCE

app.hex : app.elf
	$(Q)$(OBJCOPY) -O ihex $< $@ 

app.elf : $(built-ins) FORCE
	$(Q) echo "Linking Application:  $@"
	$(Q)$(CPP) -Wl,--start-group $(built-ins) -Wl,--end-group  -T $(cmd_file) $(LDFLAGS) -o $@ 

$(built-ins): FORCE
	$(Q)$(MAKE) $(build)=$(patsubst %/,%,$(dir $@)) \
	need-builtin=1

FORCE:

clean: 
	@rm -f *.map *.elf
	@find -name '*.[aod]' -type f -print | xargs rm -f

clean_app:
	@rm -f *.map *.elf
	@find ./application -name '*.[aod]' -type f -print  | xargs rm -f

.phony = $(PHONY)