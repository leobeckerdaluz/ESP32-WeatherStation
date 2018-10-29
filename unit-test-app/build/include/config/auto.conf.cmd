deps_config := \
	/home/bullpc/esp/esp-idf/components/app_trace/Kconfig \
	/home/bullpc/esp/esp-idf/components/aws_iot/Kconfig \
	/home/bullpc/esp/esp-idf/components/bt/Kconfig \
	/home/bullpc/esp/esp-idf/components/driver/Kconfig \
	/home/bullpc/esp/esp-idf/components/esp32/Kconfig \
	/home/bullpc/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/bullpc/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/bullpc/esp/esp-idf/components/ethernet/Kconfig \
	/home/bullpc/esp/esp-idf/components/fatfs/Kconfig \
	/home/bullpc/esp/esp-idf/components/freemodbus/Kconfig \
	/home/bullpc/esp/esp-idf/components/freertos/Kconfig \
	/home/bullpc/esp/esp-idf/components/heap/Kconfig \
	/home/bullpc/esp/esp-idf/components/http_server/Kconfig \
	/home/bullpc/esp/esp-idf/components/libsodium/Kconfig \
	/home/bullpc/esp/esp-idf/components/log/Kconfig \
	/home/bullpc/esp/esp-idf/components/lwip/Kconfig \
	/home/bullpc/esp/esp-idf/components/mbedtls/Kconfig \
	/home/bullpc/esp/esp-idf/components/mdns/Kconfig \
	/home/bullpc/esp/esp-idf/components/mqtt/Kconfig \
	/home/bullpc/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/bullpc/esp/esp-idf/components/openssl/Kconfig \
	/home/bullpc/esp/esp-idf/components/pthread/Kconfig \
	/home/bullpc/esp/esp-idf/components/spi_flash/Kconfig \
	/home/bullpc/esp/esp-idf/components/spiffs/Kconfig \
	/home/bullpc/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/bullpc/projects/ESP32-WeatherStation/unit-test-app/components/unity/Kconfig \
	/home/bullpc/esp/esp-idf/components/vfs/Kconfig \
	/home/bullpc/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/bullpc/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/bullpc/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/bullpc/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/bullpc/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
