# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/http_server/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/http_server_test -Wl,--whole-archive -lhttp_server_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += http_server_test
component-http_server_test-build: 
