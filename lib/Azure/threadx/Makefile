ARCH := cortex_m3
TOOL := gnu

obj-y += common/src/
obj-y += ports/$(ARCH)/$(TOOL)/src/

INCLUDES = -I$(AZURE_DIR)/threadx/common/inc -I$(AZURE_DIR)/threadx/ports/$(ARCH)/$(TOOL)/inc
