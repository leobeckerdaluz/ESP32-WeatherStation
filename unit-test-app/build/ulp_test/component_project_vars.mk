# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/ulp/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/ulp_test -lulp_test -lulp_test -T $(BUILD_DIR_BASE)/ulp_test/ulp_test.ld -Wl,--whole-archive -lulp_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += ulp_test
component-ulp_test-build: 
