# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/spi_flash/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/spi_flash_test -Wl,--whole-archive -lspi_flash_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += spi_flash_test
component-spi_flash_test-build: 
