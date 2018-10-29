# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/spiffs/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/spiffs_test -Wl,--whole-archive -lspiffs_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += spiffs_test
component-spiffs_test-build: 
