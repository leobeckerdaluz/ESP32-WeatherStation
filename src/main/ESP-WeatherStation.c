#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
/*LOG*/
#include "esp_err.h"
#include "esp_log.h"
/*ADC*/
#include "driver/adc.h"
#include "esp_adc_cal.h"
/*SPIFFS*/
#include "esp_spiffs.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include <string.h>
/*WIFI*/
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_attr.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_sleep.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"
#include <sys/time.h>
#include <time.h>
/*PROJECT*/
#include "ESP-WeatherStation.h"

static const char *TAG = "ESP32-WeatherStation";

/*ADC*/
esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_5;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

uint32_t voltageADCreading(){
	uint32_t adc_reading = 0;
	//Multisampling
	for (int i = 0; i < NO_OF_SAMPLES; i++) {
		if (unit == ADC_UNIT_1) {
			adc_reading += adc1_get_raw((adc1_channel_t)channel);
		} else {
			int raw;
			adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
			adc_reading += raw;
		}
	}
	adc_reading /= NO_OF_SAMPLES;
	//Convert adc_reading to voltage in mV
	uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
	ESP_LOGI(TAG, "Leitura ADC: %d", adc_reading);

	return voltage;
}
