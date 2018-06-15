#include <IOXhop_FirebaseESP32.h>
#include <WiFi.h>

// SSID do WiFi
#define WIFI_SSID "apbeck"
// PASSWORD do WiFi
#define WIFI_PASSWORD "12345679"
// Firebase Database URL
#define databaseURL "engenharia-de-software-fa3bf.firebaseio.com"
// Pino onde o Sensor está conectado
#define SENSORPIN 33
// Pino onde o Atuador está conectado
#define ACTUATORPIN 23
// Pino onde o LED de status está conectado
#define STATUSPIN 2
// Definição do caminho onde os dados do sensor serão armazenados
#define LOGSPATH "logs/"
// Definição do caminho onde a variável do atuador está armazenada
#define ACTUATORPATH "/atuador"
// Definição de quantas vezes o valor do sensor será lido até que haja o tratamento (Máx: 255)
#define TIMESTOUPLOADVALUE 10
// Tempo de intervalo entre cada leitura
#define TIMESENSORREAD 500
// Define os títulos a serem printados na Serial
#define VALUEUPLOAD "------------------Value Upload----------------"
#define VECTOR 		"---------------------Vector-------------------"
#define SORT 		"---------------------Sort---------------------"
#define ENDVECTOR 	"-------------------End Vector-----------------"
#define UPDATE 		"---------------------Update-------------------"
#define EMPTYSPACE 	"----------------------------------------------"

// Definições para a data e horário
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

unsigned long long int lastTime = 0;
unsigned long long int millissec = 0;
float temperatures[TIMESTOUPLOADVALUE];
byte contSensorReads = 0;

float ADCtoCelcius(int ADCread){
	// Resolução: 1.220703125 mV
	// Logo, 8 equivale a 1ºC

	// 5v 1024
	// V   read
	// V(volts) = 5*read/1024

	// Se 10mv equivale a 1ºC, então: 
	// T(ºC) = V*1000/10
	// Logo, T(ºC) = (5*read/4096) * 1000/10
	// Logo, T(ºC) = read * 500.0  / 4096.0

 	return ((ADCread * 500.0) / 4096.0);
}

float vectorMedian(){
	// Printa o vetor
	Serial.println(VECTOR);
	for (int i=0; i<TIMESTOUPLOADVALUE; i++){
		Serial.print("  ");
		Serial.print(i);
		Serial.print("- ");
		Serial.println(temperatures[i]);
	}

	// Ordena o vetor
	for (int i = 0; i<TIMESTOUPLOADVALUE; i++){
		for (int j = 0; j<TIMESTOUPLOADVALUE; j++){
			if (temperatures[j] > temperatures[i]){
				float tmp = temperatures[i];
				temperatures[i] = temperatures[j];
				temperatures[j] = tmp;
			}
		}
	}

	// Printa o vetor depois do sort
	Serial.println(SORT);
	for (int i=0; i<TIMESTOUPLOADVALUE; i++){
		Serial.print("  ");
		Serial.print(i);
		Serial.print("- ");
		Serial.println(temperatures[i]);
	}
	Serial.println(ENDVECTOR);

	// Define a posição do vetor a ser retornada e retorna
	byte position = (TIMESTOUPLOADVALUE/2);
	return temperatures[position];
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
	Serial.print("connecting in ");
	Serial.print(WIFI_SSID);
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
		delay(200);
	}
	Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S");
	// Serial.println(timeinfo.tm_sec);
	// Serial.println(timeinfo.tm_min);
	// Serial.println(timeinfo.tm_hour);
	// Serial.println(timeinfo.tm_mday);
	// Serial.println(timeinfo.tm_mon+1);
	// Serial.println(timeinfo.tm_year+1900);
	// Serial.println(timeinfo.tm_wday);
	// Serial.println(timeinfo.tm_yday);
	// Serial.println(timeinfo.tm_isdst);
	
	// Firebase é incializado
	Firebase.begin(databaseURL);

	// Pisca LED de status para sinalizar o final da etapa de configuração
	for (int i=0; i<10; i++){
		digitalWrite(STATUSPIN, HIGH);
		delay(80);
		digitalWrite(STATUSPIN, LOW);
		delay(80);
	}

	// Realiza o Stream da variável do atuador
	Firebase.stream(ACTUATORPATH, [](FirebaseStream stream) {
		// Caso ocorra um evento, verifica qual foi o evento
		String eventType = stream.getEvent();
		eventType.toLowerCase();
		Serial.println(UPDATE);
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
		}
		Serial.println(EMPTYSPACE);
	});  

	lastTime = millis();
}

void loop() {
	// Handle millis() {https://oshlab.com/handle-millis-overflow-arduino/}
	// if (millis() - lastTime >= TIMESENSORREAD){
		// lastTime = millis();

	// Se passou o tempo entre cada leitura, lê o sensor!
	if (millis() >= lastTime){
		lastTime += TIMESENSORREAD;
		
	    int sensorRead = analogRead(SENSORPIN);
	    Serial.print("TempADC:");
	    Serial.println(sensorRead);
	    
	    float tempCelcius = ADCtoCelcius(sensorRead);
	    Serial.print("TempConversao:");
	    Serial.println(tempCelcius);

		temperatures[contSensorReads] = tempCelcius;
		Serial.print("Time: ");
		Serial.print(millis());
		Serial.print(" / ");
	    Serial.print("Leitura ");
	    Serial.print(contSensorReads);
	    Serial.print(": ");
	    Serial.println(temperatures[contSensorReads]);
		contSensorReads++;

		// Se já leu o suficiente, realiza o tratamento estatístico
		if(contSensorReads == TIMESTOUPLOADVALUE){
			// Reseta a variável contadora
			contSensorReads = 0;

			// Formata o caminho do log no database com a pasta na raiz e a data e o horário atual
			String stringDatetime = LOGSPATH;
			stringDatetime += getDatetime();

			// Realiza o tratamento estatístico para obter a temperatura correspondente
			float tempCelcius = vectorMedian();
			
			// Converte a temperatura para uma string com precisão de 1 casa decimal somente.
			char buff[10];
			dtostrf(tempCelcius, 2, 1, buff); // min length=2 / setprecision=1
		    
			// Temperatura correspondente é colocada no database
			Firebase.setString(stringDatetime, buff);
			// Lida com algum possível erro no database
			if (Firebase.failed()) {
				Serial.print("setting /number failed:");
				Serial.println(Firebase.error());  
				return;
			}
			else{
				Serial.println(VALUEUPLOAD);
				Serial.print("value on database >> ");
				Serial.print(stringDatetime);
				Serial.print(": ");
				Serial.println(buff);
				Serial.println(EMPTYSPACE);
			}
		}
	}
}
