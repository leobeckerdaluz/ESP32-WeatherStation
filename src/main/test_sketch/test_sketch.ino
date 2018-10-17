#include "IOXhop_FirebaseESP32.h"
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
}

void loop() {
	// // Testa o getDatetime
	// String stringDatetime = getDatetime();
	// // Serial.println((String)"datetime: " + stringDatetime);

	delay(500);

	// Testa o getTemperature
	float temperature = getTemperature();
	// Serial.printf("millis: %d / temperature: %f \n", millis(), temperature)
	Serial.print((String)"millis: " + millis() + (String)" / temperature:" + temperature + (String)"\n");

	delay(500);
	
	// Testa a mediana de um vetor
	float temperatures[] = {1,2,3,4,5,6,7,8,9,10};
	float temperatures[] = {1,2,3,4,5,6,7,8,9};
	float tempCelcius = getArrayMedian(temperatures);
	Serial.printf("array median: %f", tempCelcius);
}
