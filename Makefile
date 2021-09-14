
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

LIBDIR ?= /home/carlo/temp/build
OBJDIR ?= /home/carlo/temp/build/obj

ifeq ($(COMPILE_TX),y)
include libtx_sources.mk
include libtx_port_files.mk
#create output directory
  $(shell mkdir -p $(OBJDIR)/$(THREADX_DIR))
  obj-tx := $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx))
  asm-tx := $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx))
endif

ifeq ($(COMPILE_GX),y)
include libtx_sources.mk
  $(shell mkdir -p $(OBJDIR)/$(GUIX_DIR))
  obj-gx := $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx))  
endif

 $(info $(obj-tx))

all: threadx guix

######  Compile THREADX Library #######

threadx :  $(LIBDIR)/libtx.a
		
$(LIBDIR)/libtx.a : $(obj-tx) $(asm-tx)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(obj-tx) $(asm-tx)
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
$(OBJDIR)/$(THREADX_DIR)/%.o: $(AZURE_DIR)/$(THREADX_DIR)/common/src/%.c
	@echo "compiling: $<"
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc \
	                          -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@

#port asm files
$(OBJDIR)/$(THREADX_DIR)/%.o: $(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/src/%.S 
	@echo "compiling: $<"
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc \
	                          -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@



######  Compile GUIX Library ############
guix :	$(LIBDIR)/libgx.a

$(LIBDIR)/libgx.a : $(obj-gx) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(obj-gx) 
	@echo 'Finished building target: $@'
	@echo ' '

#common Guix files
$(OBJDIR)/$(GUIX_DIR)/%.o: $(AZURE_DIR)/$(GUIX_DIR)/common/src/%.c
	@echo "compiling: $<"	
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc \
	                          -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc \
	                          -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc \
							  -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@

clean:	
	rm -rf $(LIBDIR)