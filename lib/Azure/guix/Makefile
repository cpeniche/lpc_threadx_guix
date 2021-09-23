ARCH ?= cortex_m3
TOOL ?= gnu

include ./common/Makefile
include $(PROGDIR)/defines.mk

SHELL := /bin/bash

obj_dir := guix/obj
src_tree="common/inc common/src ports/$(ARCH)/$(TOOL)/inc"
# c compiler flags for cortex m3 processor
ifeq ($(ARCH),cortex_m3)
C_FLAGS = -mcpu=cortex-m3 -march=armv7-m -mthumb -DCORE_M3 -D__NO_SYSTEM_INIT
endif

#add source directory to vpath for the implicit rules to 
#find the sources
VPATH = $(CURDIR)/common/src
INCLUDES =  -I$(CURDIR)/common/inc -I$(CURDIR)/ports/$(ARCH)/$(TOOL)/inc
INCLUDES += -I$(PROGDIR)/lib/Azure/threadx/common/inc 
INCLUDES += -I$(PROGDIR)/lib/Azure/threadx/ports/$(ARCH)/$(TOOL)/inc

#check if Azure source file exists if not create link
#create build directory for dependencies and object files
ifeq ($(findstring build,$(MAKECMDGOALS)), build)
  $(shell mkdir -p $(BUILDDIR)/$(obj_dir))
  $(shell $(PROGDIR)/scripts/create_links.sh add $(CURDIR) $(AZURE_DIR)/guix $(src_tree))
endif

build: $(LIBDIR)/guix/libgx.a

$(LIBDIR)/guix/libgx.a : $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-gx))
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $?
	@echo 'Finished building target: $@'
	@echo ' '

#common Guix files
$(BUILDDIR)/$(obj_dir)/%.o : %.c
	$(Q)$(call compile,$(CC),$<,$@)
	

ifeq ($(findstring build,$(MAKECMDGOALS)), build)	
-include $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-gx:%.o=%.d))
endif

$(BUILDDIR)/$(obj_dir)/%.d : %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))

commit:
	$(Q)$(PROGDIR)/scripts/create_links.sh del $(CURDIR) dummy $(src_tree)	

src_tree:
	$(Q)$(PROGDIR)/scripts/create_links.sh create $(CURDIR) dummy $(src_tree)