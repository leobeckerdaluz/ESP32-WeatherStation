#include ".//libraries//IOXhop_FirebaseESP32.h"
#include <WiFi.h>
#include "ESP.h"

void setup() {
	// Comunicação Serial é inicializada com Baud_rate 115200
	Serial.begin(115200);
	
	// Pino onde o LED de status está conectado é declarado como saída
	pinMode(STATUSPIN, OUTPUT);
	// Pino onde o Sensor está conectado é declarado como entrada
	pinMode(SENSORPIN, INPUT);
	
	// Pisca LED de status para sinalizar o final da etapa de configuração
	for (int i=0; i<10; i++){
		digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
		delay(50);
	}

	// --------
	//  TESTES
	// --------
	Serial.printf("----------------------------------------------------------------\n");
	Serial.printf("---------------------------- TESTES ----------------------------\n");
	Serial.printf("----------------------------------------------------------------\n");
	// --------


	// Testa o getTemperature
	int adc_read = 512;
	float temperature = ADCtoTemperature(adc_read);
	float expected_temperature = 62.5;
	Serial.printf("1- Teste (ADC to Temperature):                       ");
	if(expected_temperature == temperature)
		Serial.printf("APROVADO!!! \ntemperature (%f) == expected_temperature (%f) \n", temperature, expected_temperature);
	else
		Serial.printf("REPROVADO!!! \ntemperature (%f) == expected_temperature (%f) \n", temperature, expected_temperature);
	Serial.printf("----------------------------------------------------------------\n");


	delay(500);
	

	// Testa a mediana de um vetor IMPAR
	float v_array1[] = {1.5, 6.0, 5.7, 2.9, 3.2};
	int array_size1 = sizeof(v_array1) / sizeof(v_array1[0]);//Method
	float expected_median1 = 3.2;
	float array_median1 = getArrayMedian(v_array1, array_size1);
	Serial.printf("2- Teste (Mediana de vetor com tamanho ÍMPAR):       ");
	if(array_median1 == expected_median1)
		Serial.printf("APROVADO!!! \narrayMedian (%f) == expected (%f)\n", array_median1, expected_median1);
	else
		Serial.printf("REPROVADO!!! \narrayMedian (%f) == expected (%f)\n", array_median1, expected_median1);
	Serial.printf("----------------------------------------------------------------\n");


	delay(500);


	// Testa a mediana de um vetor PAR
	float v_array2[] = {1.0 ,5.0, 6.0, 2.0, 11.0, 4.0};
	int array_size2 = sizeof(v_array2) / sizeof(v_array2[0]);//Method
	float expected_median2 = 4.5;
	float array_median2 = getArrayMedian(v_array2, array_size2);
	Serial.printf("3- Teste (Mediana de vetor com tamanho PAR):         ");
	if(array_median1 == expected_median1)
		Serial.printf("APROVADO!!! \narrayMedian (%f) == expected (%f)\n", array_median1, expected_median1);
	else
		Serial.printf("REPROVADO!!! \narrayMedian (%f) == expected (%f)\n", array_median1, expected_median1);
	Serial.printf("----------------------------------------------------------------\n");


	delay(500);


	// Conexão com WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.printf("Connecting in %s\n", WIFI_SSID);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
		delay(300);
	}
	Serial.printf("Connected: %s\n", WiFi.localIP());


	delay(500);


	// Obtém a data atual
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo)){
		Serial.printf("tentando...\n");
	}
	String datetime = getDatetime(timeinfo);
	Serial.printf("Datetime: %s\n", datetime);
}

void loop() {}
	
