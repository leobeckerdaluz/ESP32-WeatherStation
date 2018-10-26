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

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static const char *TAG = "ESP32-WeatherStation";

/*ADC*/
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_5;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;
/*STATUSLED*/
#define STATUS_LED 2

int64_t millis_geral;

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

	// lastTime = millis();

	/* Set the GPIO as a push/pull output */
	
	for (int i=0; i<NUM_BLINKS_INIT; i++){
			/* Blink on (output high) */
			gpio_set_level(STATUS_LED, 1); 
			vTaskDelay(DELAY_BLINK_ON / portTICK_PERIOD_MS);
			gpio_set_level(STATUS_LED, 0);
			vTaskDelay(DELAY_BLINK_OFF / portTICK_PERIOD_MS);
	}
	ESP_LOGI(TAG, "All Set");

	millis_geral = esp_timer_get_time();
	ESP_LOGI(TAG, "TIMER: %lld us", millis_geral);
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
	printf("Leitura ADC: %d\tVoltage: %dmV\n", adc_reading, voltage);

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

void app_main() {
	ESP_LOGI(TAG, "Start");
	init();
	int contSensorReads=0;
	float temperatures[TIMESTOUPLOADVALUE];

	//Continuously sample ADC1
	while (1) {
		// millis_geral += 2000000;
		ESP_LOGI(TAG, "TIMER INSIDE WHILE: %lld us", esp_timer_get_time());

		uint32_t voltage = voltageADCreading();
		printf("Voltage: %dmV\n", voltage);

		float temperature = milivoltsTOcelciusLM35(voltage);
		temperatures[contSensorReads] = temperature;
		printf("Temperature: %.1f ºC\n", temperature);

		contSensorReads++;
		vTaskDelay(pdMS_TO_TICKS(500));

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
			
			// setTempOnFirebase(stringDatetime, tempCelcius);
		}

	}
	// xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
