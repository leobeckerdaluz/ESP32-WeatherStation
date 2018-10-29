# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/ethernet/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/ethernet_test -Wl,--whole-archive -lethernet_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += ethernet_test
component-ethernet_test-build: 
