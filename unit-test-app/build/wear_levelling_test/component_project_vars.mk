# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/wear_levelling/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/wear_levelling_test -Wl,--whole-archive -lwear_levelling_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += wear_levelling_test
component-wear_levelling_test-build: 
