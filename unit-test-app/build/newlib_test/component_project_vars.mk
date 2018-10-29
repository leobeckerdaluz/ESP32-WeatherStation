# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/newlib/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/newlib_test -Wl,--whole-archive -lnewlib_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += newlib_test
component-newlib_test-build: 
