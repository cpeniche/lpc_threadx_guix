
ARCH ?= cortex_m3
TOOL ?= gnu

-include ./common/src/Makefile
-include ./ports/$(ARCH)/$(TOOL)/src/Makefile
-include $(PROGDIR)/scripts/defines.mk


obj_dir := threadx/obj
src_tree = "common/inc common/src common_modules/inc common_modules/module_lib/src common_modules/module_manager/inc \
            common_modules/module_manager/src ports/$(ARCH)/$(TOOL)/src ports/$(ARCH)/$(TOOL)/inc"

# c compiler flags for cortex m3 processor
ifeq ($(ARCH),cortex_m3)
C_FLAGS = -mcpu=cortex-m3 -march=armv7-m -mthumb -DCORE_M3 -D__NO_SYSTEM_INIT
endif

#add source directory to vpath for the implicit rules to 
#find the sources
VPATH = $(CURDIR)/common/src
VPATH += $(CURDIR)/ports/$(ARCH)/$(TOOL)/src

INCLUDES =  -I$(CURDIR)/common/inc -I$(CURDIR)/ports/$(ARCH)/$(TOOL)/inc

#check if Azure source file exists if not create link
#create build directory for dependencies and object files
ifeq ($(findstring build,$(MAKECMDGOALS)), build)
  $(shell mkdir -p $(BUILDDIR)/$(obj_dir))
  $(shell $(PROGDIR)/scripts/create_links.sh add $(CURDIR) $(AZURE_DIR)/threadx $(src_tree))
endif


build: $(LIBDIR)/threadx/libtx.a

$(LIBDIR)/threadx/libtx.a : $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-tx)) $(addprefix $(BUILDDIR)/$(obj_dir)/,$(asm-tx))
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $? 
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
#$(BUILDDIR)/$(obj_dir)/%.o : %.c
#	$(Q)$(call compile,$(CC),$<,$@)

$(BUILDDIR)/$(obj_dir)%.o :
	$(Q)$(MAKE) -f $(PROGDIR)/scripts/Makefile.build obj=$(dir $@)

#port asm files
$(BUILDDIR)/$(obj_dir)/%.o : %.S 
	$(Q)$(call compile,$(CC),$<,$@)

ifeq ($(findstring build,$(MAKECMDGOALS)), build)
-include $(addprefix $(BUILDDIR)/$(obj_dir)/,$(obj-tx:%.o=%.d))
endif

$(BUILDDIR)/$(obj_dir)/%.d: %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))

ifeq ($(findstring build,$(MAKECMDGOALS)), build)
-include $(addprefix $(BUILDDIR)/$(obj_dir)/,$(asm-tx:%.o=%.d))
endif

$(BUILDDIR)/$(obj_dir)/%.d: %.S
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))

commit:
	$(Q)$(PROGDIR)/scripts/create_links.sh del $(CURDIR) dummy $(src_tree)

src_tree:
	$(Q)$(PROGDIR)/scripts/create_links.sh create $(CURDIR) dummy $(src_tree)