
ARCH ?= cortex_m3
TOOL ?= gnu

include ./common/Makefile
include ./ports/$(ARCH)/$(TOOL)/Makefile
include $(PROGDIR)/defines.mk

obj_dir := threadx/obj

# c compiler flags for cortex m3 processor
ifeq ($(ARCH),cortex_m3)
C_FLAGS = -mcpu=cortex-m3 -march=armv7-m -mthumb -DCORE_M3 -D__NO_SYSTEM_INIT
endif

#create build directory for dependencies and object files
$(shell mkdir -p $(BUILDDIR)/$(obj_dir))

#$(info after shell command)

#add source directory to vpath for the implicit rules to 
#find the sources
VPATH = $(CURDIR)/common/src
VPATH += $(CURDIR)/ports/$(ARCH)/$(TOOL)/src

INCLUDES =  -I$(CURDIR)/common/inc -I$(CURDIR)/ports/$(ARCH)/$(TOOL)/inc

#check if Azure source file exists if not create link
$(shell $(PROGDIR)/scripts/create_links.sh $(CURDIR))

$(LIBDIR)/threadx/libtx.a : $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-tx)) $(addprefix $(BUILDDIR)/$(obj_dir)/,$(asm-tx))
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $? 
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
$(BUILDDIR)/$(obj_dir)/%.o : %.c
	$(Q)$(call compile,$(CC),$<,$@)

#port asm files
$(BUILDDIR)/$(obj_dir)/%.o : %.S 
	$(Q)$(call compile,$(CC),$<,$@)

-include $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-tx:%.o=%.d))

$(BUILDDIR)/$(obj_dir)/%.d: %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))


-include $(addprefix $(BUILDDIR)/$(obj_dir)/,$(asm-tx:%.o=%.d))

$(BUILDDIR)/$(obj_dir)/%.d: %.S
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))