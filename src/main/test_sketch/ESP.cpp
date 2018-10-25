#include "IOXhop_FirebaseESP32.h"
#include "ESP.h"

void setTempOnFirebase(String stringDatetime, float tempCelcius){
	// Converte a temperatura para uma string com precisão de 1 casa decimal somente.
	char temp[10];
	dtostrf(tempCelcius, 2, 1, temp); // min length=2 / setprecision=1

	// Temperatura correspondente é colocada no database
	Firebase.setString(stringDatetime, temp);
	// Lida com algum possível erro no database
	if (Firebase.failed()) {
		Serial.println((String)"setting /number failed: " + Firebase.error());
		return;
	}
	else{
		Serial.println(VALUEUPLOAD);
		Serial.println((String)"value on database >> " + stringDatetime + (String)": " + temp);
		Serial.println(EMPTYSPACE);
	}
}


//------------------------------------------------------
//------------------------------------------------------

float ADCtoTemperature(int adc_read){
	// Resolução: 1.220703125 mV
	// Logo, 8mv equivale a 1ºC

	// 5v 1024
	// V   read
	// V(volts) = 5*read/1024

	// Se 10mv equivale a 1ºC, então: 
	// T(ºC) = V*1000/10
	// Logo, T(ºC) = (5*read/4096) * 1000/10
	// Logo, T(ºC) = read * 500.0  / 4096.0

	float temperature = (adc_read * 500.0) / 4096.0;
 	return temperature;
}

//------------------------------------------------------
//------------------------------------------------------

float getArrayMedian(float v_array[], int array_size){
	// Printa o vetor
	// Serial.println(VECTOR);
	// for (int i=0; i<array_size; i++){
	// 	Serial.print("  ");
	// 	Serial.print(i);
	// 	Serial.print("- ");
	// 	Serial.println(v_array[i]);
	// }

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

	// Printa o vetor depois do sort
	// Serial.println(SORT);
	// for (int i=0; i<array_size; i++){
	// 	Serial.print("  ");
	// 	Serial.print(i);
	// 	Serial.print("- ");
	// 	Serial.println(v_array[i]);
	// }
	// Serial.println(ENDVECTOR);

	// Define a posição do vetor a ser retornada e retorna
	float median;
	
	if ((array_size % 2) == 0){
		median = (v_array[array_size/2] + v_array[(array_size/2)-1]) / 2;
	}
	else{
		median = v_array[array_size/2];
	}

	return median;
}

//------------------------------------------------------
//------------------------------------------------------

String getDatetime(tm timeinfo){
	// Nome da variável é formatado para o formato "data/horário"
	String stringDatetime = "";

	if(timeinfo.tm_mday < 10) 	stringDatetime += "0";
	stringDatetime += timeinfo.tm_mday;

	stringDatetime += "-";

	if(timeinfo.tm_mon+1 < 10) 	stringDatetime += "0";
	stringDatetime += timeinfo.tm_mon+1;

	stringDatetime += "-";

	stringDatetime += timeinfo.tm_year+1900;
	stringDatetime += "/";

	if(timeinfo.tm_hour < 10) 	stringDatetime += "0";
	stringDatetime += timeinfo.tm_hour;

	stringDatetime += ":";

	if(timeinfo.tm_min < 10) 	stringDatetime += "0";
	stringDatetime += timeinfo.tm_min;

	stringDatetime += ":";

	if(timeinfo.tm_sec < 10) 	stringDatetime += "0";
	stringDatetime += timeinfo.tm_sec;

	// Serial.print("stringDatetime: ");
	// Serial.println(stringDatetime);

	return stringDatetime;
}
