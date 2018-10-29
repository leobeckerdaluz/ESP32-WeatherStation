# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/driver/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/driver_test -Wl,--whole-archive -ldriver_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += driver_test
component-driver_test-build: 
