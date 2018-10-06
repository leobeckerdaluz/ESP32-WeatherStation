#include "IOXhop_FirebaseESP32.h"
#include <WiFi.h>
#include "ESP.h"

// Definições para a data e horário
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

unsigned long long int lastTime = 0;
unsigned long long int millissec = 0;
float temperatures[TIMESTOUPLOADVALUE];
byte contSensorReads = 0;

float getTemp(){
	// Resolução: 1.220703125 mV
	// Logo, 8mv equivale a 1ºC

	// 5v 1024
	// V   read
	// V(volts) = 5*read/1024

	// Se 10mv equivale a 1ºC, então: 
	// T(ºC) = V*1000/10
	// Logo, T(ºC) = (5*read/4096) * 1000/10
	// Logo, T(ºC) = read * 500.0  / 4096.0

	int ADCread = analogRead(SENSORPIN);
    Serial.print("TempADC:");
    Serial.println(ADCread);

	float temperature = (ADCread * 500.0) / 4096.0;
 	return temperature;
}

float getArrayMedian(float array[TIMESTOUPLOADVALUE]){
	// Printa o vetor
	Serial.println(VECTOR);
	for (int i=0; i<TIMESTOUPLOADVALUE; i++){
		Serial.print("  ");
		Serial.print(i);
		Serial.print("- ");
		Serial.println(array[i]);
	}

	// Ordena o vetor
	for (int i = 0; i<TIMESTOUPLOADVALUE; i++){
		for (int j = 0; j<TIMESTOUPLOADVALUE; j++){
			if (array[j] > array[i]){
				float tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}

	// Printa o vetor depois do sort
	Serial.println(SORT);
	for (int i=0; i<TIMESTOUPLOADVALUE; i++){
		Serial.print("  ");
		Serial.print(i);
		Serial.print("- ");
		Serial.println(array[i]);
	}
	Serial.println(ENDVECTOR);

	// Define a posição do vetor a ser retornada e retorna
	byte position = (TIMESTOUPLOADVALUE/2);
	return array[position];
}

String getDatetime(){
	// Obtém a data atual
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo));
	
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

void setup() {
	// Pino onde o atuador está conectado é declarado como saída
	pinMode(ACTUATORPIN, OUTPUT);
	// Pino onde o LED de status está conectado é declarado como saída
	pinMode(STATUSPIN, OUTPUT);
	// Pino onde o Sensor está conectado é declarado como entrada
	pinMode(SENSORPIN, INPUT);
	
	// Comunicação Serial é inicializada com Baud_rate 115200
	Serial.begin(115200);

	// Conexão com WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	// Enquanto o WiFi não está conectado, tenta conectar.
	Serial.print((String)"connecting in " + WIFI_SSID);
	boolean statusLed = false;
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
		delay(300);
	}
	Serial.print((String)"connected: " + WiFi.localIP());
	
	// Configura os parâmetros para a busca da data e do horário atual
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	
	// Busca pela data e horário atual
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo)){
		Serial.println("Failed to obtain time");
		digitalWrite(STATUSPIN, !digitalRead(STATUSPIN));
		delay(200);
	}
	Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S");
	
	// Firebase é incializado
	Firebase.begin(databaseURL);

	// Pisca LED de status para sinalizar o final da etapa de configuração
	for (int i=0; i<10; i++){
		digitalWrite(STATUSPIN, HIGH);
		delay(50);
		digitalWrite(STATUSPIN, LOW);
		delay(50);
	}

	lastTime = millis();
}

void loop() {
	// Handle millis() {https://oshlab.com/handle-millis-overflow-arduino/}
	// if (millis() - lastTime >= TIMESENSORREAD){
		// lastTime = millis();

	// Se passou o tempo entre cada leitura, lê o sensor!
	if (millis() >= lastTime){
		lastTime += TIMESENSORREAD;

		temperatures[contSensorReads] = getTemp();
		Serial.print((String)"Time: " + millis() + (String)" / Leitura " + contSensorReads + (String)": " + temperatures[contSensorReads] + (String)"\n");
		contSensorReads++;

		// Se já leu o suficiente, realiza o tratamento estatístico
		if(contSensorReads == TIMESTOUPLOADVALUE){
			// Reseta a variável contadora
			contSensorReads = 0;

			// Formata o caminho do log no database com a pasta na raiz e a data e o horário atual
			String stringDatetime = LOGSPATH + getDatetime();
			
			// Realiza o tratamento estatístico para obter a temperatura correspondente
			float tempCelcius = getArrayMedian(temperatures);
			
			setTempOnFirebase(stringDatetime, tempCelcius);
		}
	}
}
