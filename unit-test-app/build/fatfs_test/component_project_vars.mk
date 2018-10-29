# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/fatfs/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/fatfs_test -Wl,--whole-archive -lfatfs_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += fatfs_test
component-fatfs_test-build: 
