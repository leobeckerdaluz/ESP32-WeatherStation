# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/app_update/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/app_update_test -Wl,--whole-archive -lapp_update_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += app_update_test
component-app_update_test-build: 
