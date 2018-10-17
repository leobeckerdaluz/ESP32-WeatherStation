#include "IOXhop_FirebaseESP32.h"
#include <WiFi.h>
#include "ESP.h"

// Definições para a data e horário
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

unsigned long long int lastTime = 0;
float temperatures[TIMESTOUPLOADVALUE];
byte contSensorReads = 0;


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

		temperatures[contSensorReads] = getTemperature();
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
