# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/nvs_flash/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/nvs_flash_test -Wl,--whole-archive -lnvs_flash_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += nvs_flash_test
component-nvs_flash_test-build: 
