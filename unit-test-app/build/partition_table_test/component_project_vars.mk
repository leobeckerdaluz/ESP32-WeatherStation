# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/partition_table/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/partition_table_test -Wl,--whole-archive -lpartition_table_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += partition_table_test
component-partition_table_test-build: 
