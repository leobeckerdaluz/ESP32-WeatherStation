# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/expat/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/expat_test -Wl,--whole-archive -lexpat_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += expat_test
component-expat_test-build: 
