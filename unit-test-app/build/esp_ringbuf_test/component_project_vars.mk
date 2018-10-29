# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/esp_ringbuf/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/esp_ringbuf_test -Wl,--whole-archive -lesp_ringbuf_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += esp_ringbuf_test
component-esp_ringbuf_test-build: 
