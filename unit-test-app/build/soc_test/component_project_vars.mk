# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/soc/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/soc_test -Wl,--whole-archive -lsoc_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += soc_test
component-soc_test-build: 
