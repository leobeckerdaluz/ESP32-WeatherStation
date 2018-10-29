# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/mbedtls/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/mbedtls_test -Wl,--whole-archive -lmbedtls_test -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += mbedtls_test
component-mbedtls_test-build: 
