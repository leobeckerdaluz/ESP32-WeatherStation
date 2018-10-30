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
/* TESTS */
#include "unity.h"
#include "unity_config.h"
/*PROJECT*/
#include "ESP-WeatherStation.h"

static const char *TAG = "ESP32-WeatherStation";

void unityTask(void *pvParameters) {
    vTaskDelay(2); /* Delay a bit to let the main task be deleted */
    unity_run_menu(); /* Doesn't return */
}

void app_main() {
    // Note: if unpinning this task, change the way run times are calculated in
    // unity_platform
    xTaskCreatePinnedToCore(unityTask, "unityTask", UNITY_FREERTOS_STACK_SIZE, NULL,
                            UNITY_FREERTOS_PRIORITY, NULL, UNITY_FREERTOS_CPU);
}

TEST_CASE("voltage to celcius PASS", "[milivoltsTOcelciusLM35 function]"){
	ESP_LOGI(TAG, "--------- voltage to celcius PASS ----------");

	uint32_t voltage;
	float expected_temp;
	float temp;

	voltage = 85;
	expected_temp = 8.5;
	temp = milivoltsTOcelciusLM35(voltage);
	ESP_LOGI(TAG, "Voltage: %d mV", voltage);
	ESP_LOGI(TAG, "Temperature: %.1f ºC", temp);
	ESP_LOGI(TAG, "Expected Temperature == %.1f ºC", expected_temp);
	TEST_ASSERT(expected_temp == temp);

	ESP_LOGI(TAG, "--------------------------------------------");
}

TEST_CASE("voltage to celcius FAIL", "[milivoltsTOcelciusLM35 function]"){
	ESP_LOGI(TAG, "--------- voltage to celcius FAIL ----------");

	uint32_t voltage;
	float expected_temp;
	float temp;

	voltage = 85;
	expected_temp = 5.0;
	temp = milivoltsTOcelciusLM35(voltage);
	ESP_LOGI(TAG, "Voltage: %d mV", voltage);
	ESP_LOGI(TAG, "Temperature: %.1f ºC", temp);
	ESP_LOGI(TAG, "Force Error Temperature: %.1f ºC", expected_temp);
	TEST_ASSERT(expected_temp != temp);

	ESP_LOGI(TAG, "--------------------------------------------");
}

TEST_CASE("array median PASS", "[getArrayMedian function]"){
	ESP_LOGI(TAG, "------------ array median PASS -------------");

	float array_v[7] = {5.2, 6.3, 8.5, 9.2, 5.1, 6.5, 5.9};
	int array_size = 7;
	float expected_median = 6.3;
	float array_median = getArrayMedian(array_v, array_size);

	ESP_LOGI(TAG, "Array Sorted: ");
	int i;
	for (i=0; i<array_size; i++){
		ESP_LOGI(TAG, "\tArray[%d]: %f", i, array_v[i]);
	}
	ESP_LOGI(TAG, "Array Median: %f", array_median);
	ESP_LOGI(TAG, "Expected Median: %f ºC", expected_median);
	TEST_ASSERT(expected_median == array_median);

	ESP_LOGI(TAG, "--------------------------------------------");
}

TEST_CASE("array median FAIL", "[getArrayMedian function]"){
	ESP_LOGI(TAG, "------------ array median FAIL -------------");

	float array_v[7] = {5.2, 6.3, 8.5, 9.2, 5.1, 6.5, 5.9};
	int array_size = 7;
	float expected_median = 5.9;
	float array_median = getArrayMedian(array_v, array_size);

	ESP_LOGI(TAG, "Array Sorted: ");
	int i;
	for (i=0; i<array_size; i++){
		ESP_LOGI(TAG, "\tArray[%d]: %f", i, array_v[i]);
	}
	ESP_LOGI(TAG, "Array Median: %f", array_median);
	ESP_LOGI(TAG, "Force Error Median: %f ºC", expected_median);
	TEST_ASSERT(expected_median != array_median);

	ESP_LOGI(TAG, "--------------------------------------------");
}

TEST_CASE("file system PASS", "[]"){
	ESP_LOGI(TAG, "------------- file system PASS -------------");
	FILE* f;

	// init_spiffs();

	// // Open file to write
	// ESP_LOGI(TAG, "Opening file");
	// f = fopen("/spiffs/test.txt", "w");
	// if (f == NULL) {
 //        ESP_LOGE(TAG, "Failed to open file for writing");
 //        return;
 //    }
 //    fprintf(f, "teste");
 //    fclose(f);
	// ESP_LOGI(TAG, "File written Closed");





	// ESP_LOGI(TAG, "Reading file");
 //    f = fopen("/spiffs/test.txt", "r");
 //    if (f == NULL) {
 //        ESP_LOGE(TAG, "Failed to open file for reading");
 //        return;
 //    }
 //    char line[64];
 //    while(fgets(line, sizeof(line), f)){
 //        // strip newline
 //        char* pos = strchr(line, '\n');
 //        if (pos) {
 //            *pos = '\0';
 //        }
 //        ESP_LOGI(TAG, "Read from file: '%s'", line);
    
 //        //do something with buf here;
 //        //The input of fgets will be NULL as soon 
 //        //as its input fp has been fully read, then exit the loop  
 //    }
 //    fclose(f);




 //    // All done, unmount partition and disable SPIFFS
 //    esp_vfs_spiffs_unregister(NULL);
 //    ESP_LOGI(TAG, "SPIFFS unmounted");


	ESP_LOGI(TAG, "--------------------------------------------");
}

TEST_CASE("datetime to string PASS", "[datetime_to_string function]"){
	ESP_LOGI(TAG, "--------- datetime to string PASS ----------");

	struct tm timeinfo;
	timeinfo.tm_mday = 29;  // dia: 29
	timeinfo.tm_mon = 9;    // mês: outubro
	timeinfo.tm_year = 118; // ano: 2018
	timeinfo.tm_hour = 20;  // hora: 20
	timeinfo.tm_min = 36;   // min: 36
	timeinfo.tm_sec = 0;    // seg: 00
	char* string_datetime = datetime_to_string(timeinfo);
	ESP_LOGI(TAG, "got datetime: %s", string_datetime);
	
	const char* expected_string_datetime = "29-10-18/20:36:00";
	ESP_LOGI(TAG, "expected datetime: %s", string_datetime);

	TEST_ASSERT(expected_string_datetime == string_datetime);

	ESP_LOGI(TAG, "--------------------------------------------");
}