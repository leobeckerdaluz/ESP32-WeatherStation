# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/vfs/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/vfs_test -Wl,--whole-archive -lvfs_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += vfs_test
component-vfs_test-build: 
