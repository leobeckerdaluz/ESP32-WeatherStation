/* Blink Example

	 This example code is in the Public Domain (or CC0 licensed, at your option.)

	 Unless required by applicable law or agreed to in writing, this
	 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	 CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
/*PROJECT*/
#include "ESP-WeatherStation.h"
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



/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;
/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

static const char *TAG = "ESP32-WeatherStation";

/*ADC*/
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_5;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

/*NTP*/
static void obtain_time(void);
static void initialise_wifi(void);
static esp_err_t event_handler(void *ctx, system_event_t *event);
static void check_efuse();
static void print_char_val_type(esp_adc_cal_value_t val_type);
static void init();
static void getDatetime();
static uint32_t voltageADCreading();
static float milivoltsTOcelciusLM35(uint32_t voltage);
static float getArrayMedian(float v_array[], int array_size);

static void check_efuse(){
	//Check TP is burned into eFuse
	if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
		printf("eFuse Two Point: Supported\n");
	else
		printf("eFuse Two Point: NOT supported\n");

	//Check Vref is burned into eFuse
	if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
		printf("eFuse Vref: Supported\n");
	else
		printf("eFuse Vref: NOT supported\n");
}

static void print_char_val_type(esp_adc_cal_value_t val_type){
	if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
		printf("Characterized using Two Point Value\n");
	else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
		printf("Characterized using eFuse Vref\n");
	else
		printf("Characterized using Default Vref\n");
}

void init() {
	ESP_LOGI(TAG, "Setting all parameters");

	/*SET STATUS LED PIN*/
	gpio_pad_select_gpio(STATUS_LED);
	gpio_set_direction(STATUS_LED, GPIO_MODE_OUTPUT);

	/*SET ADC*/
	//Check if Two Point or Vref are burned into eFuse
	check_efuse();
	//Configure ADC
	if (unit == ADC_UNIT_1) {
		adc1_config_width(ADC_WIDTH_BIT_12);
		adc1_config_channel_atten(channel, atten);
	} else {
		adc2_config_channel_atten((adc2_channel_t)channel, atten);
	}
	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
	print_char_val_type(val_type);




	/*SPIFFS*/
	ESP_LOGI(TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }



	// // Conexão com WiFi
	// WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	// // Enquanto o WiFi não está conectado, tenta conectar.
	// Serial.print((String)"connecting in " + WIFI_SSID);
	// boolean statusLed = false;
	// while (WiFi.status() != WL_CONNECTED) {
	// 	Serial.print(".");
	// 	digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
	// 	delay(300);
	// }
	// Serial.print((String)"connected: " + WiFi.localIP());
	
	// // Configura os parâmetros para a busca da data e do horário atual
	// configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	
	// // Busca pela data e horário atual
	// struct tm timeinfo;
	// while(!getLocalTime(&timeinfo)){
	// 	Serial.println("Failed to obtain time");
	// 	digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
	// 	delay(200);
	// }
	// Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S");
	
	// // Firebase é incializado
	// Firebase.begin(databaseURL);

	// // Pisca LED de status para sinalizar o final da etapa de configuração
	// for (int i=0; i<10; i++){
	// 	digitalWrite(STATUSPIN, HIGH);
	// 	delay(50);
	// 	digitalWrite(STATUSPIN, LOW);
	// 	delay(50);
	// }

	/* Set the GPIO as a push/pull output */
	
	for (int i=0; i<NUM_BLINKS_INIT; i++){
			/* Blink on (output high) */
			gpio_set_level(STATUS_LED, 1); 
			vTaskDelay(DELAY_BLINK_ON / portTICK_PERIOD_MS);
			gpio_set_level(STATUS_LED, 0);
			vTaskDelay(DELAY_BLINK_OFF / portTICK_PERIOD_MS);
	}
	ESP_LOGI(TAG, "All Set");

	// millis_geral = esp_timer_get_time();
	// ESP_LOGI(TAG, "TIMER: %lld us", millis_geral);
}

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
	ESP_LOGI(TAG, "Leitura ADC: %d\tVoltage: %dmV\n", adc_reading, voltage);

	return voltage;
}

float milivoltsTOcelciusLM35(uint32_t voltage){
	float temperature = (voltage/10.0);
	return temperature;
}

float getArrayMedian(float v_array[], int array_size){
	// Ordena o vetor
	for (int i = 0; i<array_size; i++){
		for (int j = 0; j<array_size; j++){
			if (v_array[j] > v_array[i]){
				float tmp = v_array[i];
				v_array[i] = v_array[j];
				v_array[j] = tmp;
			}
		}
	}

	// Define a posição do vetor a ser retornada e retorna
	float median;
	
	if ((array_size % 2) == 0)
		median = (v_array[array_size/2] + v_array[(array_size/2)-1]) / 2;
	else
		median = v_array[array_size/2];

	return median;
}


static void obtain_time(void){
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_wifi();
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);
    
    // initialize_sntp
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    ESP_ERROR_CHECK( esp_wifi_stop() );
}

static void initialise_wifi(void){
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static esp_err_t event_handler(void *ctx, system_event_t *event){
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void getDatetime(){
	time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
    char strftime_buf[64];

    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
}

void app_main() {
	init();

	int contSensorReads=0;
	float temperatures[TIMESTOUPLOADVALUE];
	FILE* f;

	// First create a file.
	ESP_LOGI(TAG, "Opening file");
	f = fopen("/spiffs/hello.txt", "w");

	int j;
	for(j=0; j<30; j++){
		ESP_LOGI(TAG, "TIMER INSIDE WHILE: %lld us", esp_timer_get_time());

		uint32_t voltage = voltageADCreading();
		ESP_LOGI(TAG, "Voltage: %dmV\n", voltage);

		float temperature = milivoltsTOcelciusLM35(voltage);
		temperatures[contSensorReads] = temperature;
		ESP_LOGI(TAG, "Temperature: %.1f ºC\n", temperature);

		contSensorReads++;
		vTaskDelay(pdMS_TO_TICKS(300));

		// Se já leu o suficiente, realiza o tratamento estatístico
		if(contSensorReads == TIMESTOUPLOADVALUE){
			ESP_LOGI(TAG, "contSensorReads == TIMESTOUPLOADVALUE");
			// Reseta a variável contadora
			contSensorReads = 0;

			// Formata o caminho do log no database com a pasta na raiz e a data e o horário atual
			// String stringDatetime = LOGSPATH + getDatetime();

			// int i;
			// for(i=0; i<TIMESTOUPLOADVALUE; i++){
				// printf("VETOR TEMP: %f\n", temperatures[i]);
			// }
			
			// Realiza o tratamento estatístico para obter a temperatura correspondente
			float tempCelcius = getArrayMedian(temperatures, TIMESTOUPLOADVALUE);
			printf("Temperatura Final: %.1f\n", tempCelcius);

			/*Writing on File*/
			if (f == NULL) {
		        ESP_LOGE(TAG, "Failed to open file for writing");
		        return;
		    }
		    fprintf(f, "Temperatura Final: %.1f\n", tempCelcius);
		    ESP_LOGI(TAG, "File written");
			
			// setTempOnFirebase(stringDatetime, tempCelcius);
		}
	}
	fclose(f);
	ESP_LOGI(TAG, "File written Closed");




    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/hello.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    while(fgets(line, sizeof(line), f)){
        // strip newline
        char* pos = strchr(line, '\n');
        if (pos) {
            *pos = '\0';
        }
        printf("TOMANOCU MEU\n");
        ESP_LOGI(TAG, "Read from file: '%s'", line);
    
        //do something with buf here;
        //The input of fgets will be NULL as soon 
        //as its input fp has been fully read, then exit the loop  
    }
    fclose(f);




    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}