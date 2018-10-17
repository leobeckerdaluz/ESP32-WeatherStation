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

	Serial.printf("---------------------------------------------\n");
	delay(500);

	int adc_read = 1023;
	// Testa o getTemperature
	float temperature = ADCtoTemperature(adc_read);
	// Serial.printf("millis: %d / temperature: %f \n", millis(), temperature);
	Serial.print((String)"millis: " + millis() + (String)" / temperature:" + temperature + (String)"\n");

	Serial.printf("---------------------------------------------\n");
	delay(500);
	
	// Testa a mediana de um vetor IMPAR
	float v_array1[] = {1.5, 6.0, 5.7, 2.9, 3.2, 10.2};
	int array_size1 = sizeof(v_array1) / sizeof(v_array1[0]);//Method
	float expected_median1 = 4.45;
	float array_median1 = getArrayMedian(v_array1, array_size1);
	Serial.printf("array median: %f \n", array_median1);
	if(array_median1 == expected_median1)
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: APROVADO! %f == %f\n", array_median1, expected_median1);
	else
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: REPROVADO! %f != %f\n", array_median1, expected_median1);

	Serial.printf("---------------------------------------------\n");
	delay(500);

	// Testa a mediana de um vetor PAR
	float v_array2[] = {1.0 ,5.0, 6.0, 2.0, 11.0, 4.0};
	int array_size2 = sizeof(v_array2) / sizeof(v_array2[0]);//Method
	float expected_median2 = 4.5;
	float array_median2 = getArrayMedian(v_array2, array_size2);
	Serial.printf("array median: %f \n", array_median2);
	if(array_median2 == expected_median2){
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: APROVADO! %f == %f\n", array_median2, expected_median2);
	}
	else{
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: REPROVADO! %f != %f\n", array_median2, expected_median2);
	}

	// Testa se os valores estão sendo armazenados float
	certo v_array2[] = {1.0 ,5.0, 6.0, 2.0, 11.0, 4.0};
	int array_size2 = sizeof(v_array2) / sizeof(v_array2[0]);//Method
	float expected_median2 = 4.5;
	float array_median2 = getArrayMedian(v_array2, array_size2);
	Serial.printf("array median: %f \n", array_median2);
	if(array_median2 == expected_median2){
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: APROVADO! %f == %f\n", array_median2, expected_median2);
	}
	else{
		Serial.printf("Teste de mediana de um vetor de tamanho ímpar: REPROVADO! %f != %f\n", array_median2, expected_median2);
	}
}
