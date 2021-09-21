
-include app_sources.mk
-include ../defines.mk

LIBS = 	libtx.a libgx.a liblpc.a
LINKER = arm-none-eabi-ld
cmd_file = gcc_arm.ld
APPDIR = app
VPATH += $(CURDIR)/src
INCLUDES += -I$(CURDIR)/inc
$(shell mkdir -p $(OBJDIR)/$(APPDIR))
xxx=example

#$(info $(INCLUDES))

app.elf : $(addprefix $(OBJDIR)/$(APP)/,$(obj-app)) $(addprefix $(OBJDIR)/,$(LIBS))
	$(Q)$(LINKER) $< -T $(cmd_file)  -o $@

$(OBJDIR)/$(APP)/%.o : %.c
	$(Q)$(call compile,$(CC),$<,$@)

$(OBJDIR)/$(APP)/%.o : %.S 
	$(Q)$(call compile,$(CC),$<,$@)

$(OBJDIR)/$(APP)/%.o : %.cpp
	$(Q)$(call compile,$(CPP),$<,$@)

-include $(addprefix $(OBJDIR)/$(APPDIR)/,$(obj-app:%.o=%.d))

$(OBJDIR)/$(APPDIR)/%.d : %.c
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))
	
$(OBJDIR)/$(APPDIR)/%.d : %.S
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))

$(OBJDIR)/$(APPDIR)/%.d : %.cpp
	$(Q)$(call dependencies,$@,$(@:%.d=%.o))