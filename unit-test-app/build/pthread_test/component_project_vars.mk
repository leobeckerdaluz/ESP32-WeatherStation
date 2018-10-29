# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/pthread/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/pthread_test -Wl,--whole-archive -lpthread_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += pthread_test
component-pthread_test-build: 
