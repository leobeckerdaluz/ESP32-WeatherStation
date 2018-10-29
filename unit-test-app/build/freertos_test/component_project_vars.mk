# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/freertos/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/freertos_test -Wl,--whole-archive -lfreertos_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += freertos_test
component-freertos_test-build: 
