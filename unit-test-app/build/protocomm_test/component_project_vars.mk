# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/protocomm/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/protocomm_test -Wl,--whole-archive -lprotocomm_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += protocomm_test
component-protocomm_test-build: 
