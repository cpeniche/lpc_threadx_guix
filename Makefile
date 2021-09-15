
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

obj-tx ?=
asm-tx ?=
obj-gx ?=
asm-gx ?=


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
# append directories to the obj-tx list and asm-tx list
  obj-tx := $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(obj-tx))
  asm-tx := $(addprefix $(OBJDIR)/$(THREADX_DIR)/,$(asm-tx))
endif

ifeq ($(MAKECMDGOALS),libgx)
include sources.mk
include libtx_port_files.mk
# append directories to the obj-gx list
  obj-gx := $(addprefix $(OBJDIR)/$(GUIX_DIR)/,$(obj-gx))  
endif

libtx : $(LIBDIR)/libtx.a

######  Compile THREADX Library #######
$(LIBDIR)/libtx.a : $(obj-tx) $(asm-tx)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(obj-tx) $(asm-tx)
	@echo 'Finished building target: $@'
	@echo ' '

#common threadx files
$(OBJDIR)/$(THREADX_DIR)/%.o: $(AZURE_DIR)/$(THREADX_DIR)/common/src/%.c
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@

#port asm files
$(OBJDIR)/$(THREADX_DIR)/%.o: $(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/src/%.S 
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@


######  Compile GUIX Library ############

libgx : $(LIBDIR)/libgx.a


$(LIBDIR)/libgx.a : $(obj-gx) 
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross Archiver'
	$(Q)$(AR) -r $@ $(obj-gx) 
	@echo 'Finished building target: $@'
	@echo ' '

#common Guix files
$(OBJDIR)/$(GUIX_DIR)/%.o: $(AZURE_DIR)/$(GUIX_DIR)/common/src/%.c                         
	@echo "[CC]: $<"
	$(Q)$(CC) -c $(CFLAGS) $< -I$(AZURE_DIR)/$(GUIX_DIR)/common/inc -I$(AZURE_DIR)/$(GUIX_DIR)/$(arch_cpu)/inc \
                              -I$(AZURE_DIR)/$(THREADX_DIR)/common/inc -I$(AZURE_DIR)/$(THREADX_DIR)/$(arch_cpu)/inc -o $@
	
#crate threadx output directory
$(obj-tx) $(asm-tx) : | $(OBJDIR)/$(THREADX_DIR)

$(OBJDIR)/$(THREADX_DIR) :
	mkdir -p $(OBJDIR)/$(THREADX_DIR)
	
#create guix output directory
$(obj-gx) : | $(OBJDIR)/$(GUIX_DIR)

$(OBJDIR)/$(GUIX_DIR) :
	mkdir -p $(OBJDIR)/$(GUIX_DIR)

clean:	
	rm -rf $(LIBDIR)