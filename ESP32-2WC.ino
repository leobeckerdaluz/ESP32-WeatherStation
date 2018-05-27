#include <IOXhop_FirebaseESP32.h>
#include <WiFi.h>

// Tempo de espera para a comparação com millis
#define defineTimeSensorRead 5000
// SSID do WiFi
#define WIFI_SSID "CS6"
// PASSWORD do WiFi
#define WIFI_PASSWORD "luisdaluz6"
// Firebase Database URL
#define databaseURL "engenharia-de-software-fa3bf.firebaseio.com"
// Pino onde o Atuador está conectado
#define ACTUATORPIN 2
// Pino onde o LED de status está conectado
#define STATUSPIN 2
// Definição do caminho onde os dados do sensor serão armazenados
#define LOGSPATH "logs/"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

unsigned long long int MILLISSEC = 0;
int n = 0;

void setup() {
	// Pino onde o atuador está conectado é declarado como saída
	pinMode(ACTUATORPIN, OUTPUT);
	
	// Comunicação Serial é inicializada com Baud_rate 9600
	Serial.begin(9600);
	
	// Conexão com WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("connecting");

	// Enquanto o WiFi não está conectado, tenta conectar a cada 300ms
	boolean statusLed = false;
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		if(statusLed){
			digitalWrite(STATUSPIN, LOW);
			statusLed = false;
		}
		else{
			digitalWrite(STATUSPIN, HIGH);
			statusLed = true;
		}
		delay(300);
	}
	Serial.print("connected: ");
	Serial.println(WiFi.localIP());
	Serial.println("");
	
	// Configura os parâmetros para a busca da data e do horário atual
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	
	// Busca pela data e horário atual
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo)){
		Serial.println("Failed to obtain time");
		if(statusLed){
			digitalWrite(STATUSPIN, LOW);
			statusLed = false;
		}
		else{
			digitalWrite(STATUSPIN, HIGH);
			statusLed = true;
		}
		delay(100);
	}
	Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S");

	// Firebase é incializado
	Firebase.begin(databaseURL);

  for (int i=0; i<10; i++){
    digitalWrite(STATUSPIN, HIGH);
    delay(50);
    digitalWrite(STATUSPIN, HIGH);
    delay(50);
  }
	// Realiza o Stream da variável do atuador
	Firebase.stream("/atuador", [](FirebaseStream stream) {
		// Caso ocorra um evento, verifica qual foi o evento
		String eventType = stream.getEvent();
		eventType.toLowerCase();
		Serial.print("event: ");
		Serial.println(eventType);
		
		// Se o evento foi um "put" em "/atuador", verifica o estado atual da variável
		if (eventType == "put") {
			// Variável recebe o valor atual da variável no database
			String statusActuator = stream.getDataString();
			// Printa o valor atual da variável na Serial
			Serial.print("Atualizado: ");
			Serial.println(statusActuator);
			// Conforme o valor da variável, seta a porta lógica corresponde ao atuador
			if(statusActuator == "true"){
				digitalWrite(ACTUATORPIN, HIGH);
			}else if(statusActuator == "false"){
				digitalWrite(ACTUATORPIN, LOW);
			}
			// String path = stream.getPath();
			// String data = stream.getDataString();
		}
	});  

	MILLISSEC = millis();
}

void loop() {
	if(millis() > MILLISSEC){
	    MILLISSEC += defineTimeSensorRead;
	    Serial.println(millis());

		// Obtém a data atual
		struct tm timeinfo;
		while(!getLocalTime(&timeinfo)){
		  // Serial.println("Failed to obtain time");
		  // return;
		}
		Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S"); 
		
		// Nome da variável é formatado para o formato "data/horário"
		String uploadTime = LOGSPATH;

		if(timeinfo.tm_mday < 10){
			uploadTime += "0";
		}
		uploadTime += timeinfo.tm_mday;

		uploadTime += "-";

		if(timeinfo.tm_mon+1 < 10){
			uploadTime += "0";
		}
		uploadTime += timeinfo.tm_mon+1;

		uploadTime += "-";

		uploadTime += timeinfo.tm_year+1900;
		uploadTime += "/";

		if(timeinfo.tm_hour < 10){
			uploadTime += "0";
		}
		uploadTime += timeinfo.tm_hour;

		uploadTime += ":";

		if(timeinfo.tm_min < 10){
			uploadTime += "0";
		}
		uploadTime += timeinfo.tm_min;

		uploadTime += ":";

		if(timeinfo.tm_sec < 10){
			uploadTime += "0";
		}
		uploadTime += timeinfo.tm_sec;

		Serial.println("uploadTime: ");
		Serial.println(uploadTime);
		Serial.println("------------------------");

		// Número 0 é colocado no banco de dados.
		// O nome da variável está no formato "data/horário"
		Firebase.setFloat(uploadTime, n);
		// Lida com algum possível erro
		if (Firebase.failed()) {
			Serial.print("setting /number failed:");
			Serial.println(Firebase.error());  
			return;
		}

	}
}
