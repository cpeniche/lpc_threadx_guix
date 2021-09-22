
-include app_sources.mk
-include ../defines.mk

LIBS = 	tx gx lpc
LINKER = $(CROSS_COMPILE)ld
cmd_file = gcc_arm.ld
VPATH += $(CURDIR)/src $(LIBDIR)
INCLUDES += -I$(CURDIR)/inc
LINKER_FLAGS := -L~/tools/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/lib \
	-L~/tools/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1 -Xlinker --gc-sections -Wl,-Map,"app.map"

$(info $(VPATH))

$(shell mkdir -p $(OBJDIR)/$(APPDIR))

$(LIBDIR)/app.elf : $(addprefix $(OBJDIR)/$(APPDIR)/,$(obj-app)) -ltx -lgx -llpc
	$(Q)$(CPP) $^ -T $(cmd_file) $(LINKER_FLAGS) -L$(LIBDIR) -o$@ $(addprefix -l,$(LIBS))

$(OBJDIR)/$(APPDIR)/%.o : %.c
	$(Q)$(call compile,$(CC),$<,$@)

$(OBJDIR)/$(APPDIR)/%.o : %.S
	$(Q)$(call compile,$(CC),$<,$@)

$(OBJDIR)/$(APPDIR)/%.o : %.cpp
	$(Q)$(call compile,$(CPP),$<,$@)

-include $(addprefix $(OBJDIR)/$(APPDIR)/,$(obj-app:%.o=%.d))

$(OBJDIR)/$(APPDIR)/%.d : %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))
	
$(OBJDIR)/$(APPDIR)/%.d : %.S
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))

$(OBJDIR)/$(APPDIR)/%.d : %.cpp
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))