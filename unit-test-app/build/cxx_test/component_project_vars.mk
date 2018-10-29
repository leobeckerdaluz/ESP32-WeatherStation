# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/cxx/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/cxx_test -Wl,--whole-archive -lcxx_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += cxx_test
component-cxx_test-build: 
