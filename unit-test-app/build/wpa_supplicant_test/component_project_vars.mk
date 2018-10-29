# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/wpa_supplicant/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/wpa_supplicant_test -Wl,--whole-archive -lwpa_supplicant_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += wpa_supplicant_test
component-wpa_supplicant_test-build: 
