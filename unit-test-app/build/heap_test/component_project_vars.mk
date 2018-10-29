# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/heap/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/heap_test -Wl,--whole-archive -lheap_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += heap_test
component-heap_test-build: 
