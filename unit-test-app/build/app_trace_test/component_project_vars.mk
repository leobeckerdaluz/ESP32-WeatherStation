# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/app_trace/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/app_trace_test -Wl,--whole-archive -lapp_trace_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += app_trace_test
component-app_trace_test-build: 
