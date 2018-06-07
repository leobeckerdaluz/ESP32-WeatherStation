#include <IOXhop_FirebaseESP32.h>
#include <WiFi.h>

// SSID do WiFi
#define WIFI_SSID "Cagghetto"
// PASSWORD do WiFi
#define WIFI_PASSWORD "12345679"
// Firebase Database URL
#define databaseURL "engenharia-de-software-fa3bf.firebaseio.com"
// Pino onde o Sensor está conectado
#define SENSORPIN 4
// Pino onde o Atuador está conectado
#define ACTUATORPIN 2
// Pino onde o LED de status está conectado
#define STATUSPIN 2
// Definição do caminho onde os dados do sensor serão armazenados
#define LOGSPATH "logs/"
// Definição do caminho onde a variável do atuador está armazenada
#define ACTUATORPATH "/atuador"
// Definição de quantas vezes o valor do sensor será lido até que haja o tratamento
#define TIMESTOUPLOADVALUE 10
// Tempo de intervalo entre cada leitura
#define TIMESENSORREAD 1000
// Define os títulos a serem printados na Serial
#define VALUEUPLOAD "------------------Value Upload----------------"
#define VECTOR 		"---------------------Vector-------------------"
#define SORT 		"---------------------Sort---------------------"
#define ENDVECTOR 	"-------------------End Vector-----------------"
#define UPDATE 		"---------------------Update-------------------"
#define EMPTYSPACE 	"----------------------------------------------"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

unsigned long long int millissec = 0;
int temperatures[TIMESTOUPLOADVALUE];
byte contSensorReads = 0;
// byte teste = 0;

int vectorMedian(){
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
				int tmp = temperatures[i];
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
	// Serial.print("Returning position: ");
	// Serial.print(position);
	// Serial.print(" / Value: ");
	// Serial.println(temperatures[position]);

	return temperatures[position];
}

String getDatetime(){
	// Obtém a data atual
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo));
	// Serial.println("----------------------------");
	// Serial.println(&timeinfo, "%b-%d-%Y/%H:%M:%S"); 
	
	// Nome da variável é formatado para o formato "data/horário"
	String stringDatetime = LOGSPATH;

	if(timeinfo.tm_mday < 10){
		stringDatetime += "0";
	}
	stringDatetime += timeinfo.tm_mday;

	stringDatetime += "-";

	if(timeinfo.tm_mon+1 < 10){
		stringDatetime += "0";
	}
	stringDatetime += timeinfo.tm_mon+1;

	stringDatetime += "-";

	stringDatetime += timeinfo.tm_year+1900;
	stringDatetime += "/";

	if(timeinfo.tm_hour < 10){
		stringDatetime += "0";
	}
	stringDatetime += timeinfo.tm_hour-5;

	stringDatetime += ":";

	if(timeinfo.tm_min < 10){
		stringDatetime += "0";
	}
	stringDatetime += timeinfo.tm_min;

	stringDatetime += ":";

	if(timeinfo.tm_sec < 10){
		stringDatetime += "0";
	}
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
	
	// Conexão com WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	// Comunicação Serial é inicializada com Baud_rate 115200
	Serial.begin(115200);
	
	// Enquanto o WiFi não está conectado, tenta conectar.
	Serial.print("connecting");
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

	millissec = millis();
}

void loop() {
	if(millis() > millissec){
	    millissec += TIMESENSORREAD;
		
	    int sensorRead;// = analogRead(SENSORPIN);
	    // // Gerador de valores
	    // if(teste==0){
	    // 	sensorRead = millis();
	    // 	teste = 1;
	    // }
	    // else if(teste == 1){
	    // 	sensorRead = millis() - TIMESENSORREAD;
	    // 	teste = 2;
	    // }
	    // else{
	    // 	sensorRead = millissec - 6543;
	    // 	teste = 0;
	    // }

		temperatures[contSensorReads] = sensorRead;
		Serial.print("Time: ");
		Serial.print(millis());
		Serial.print(" / ");
	    Serial.print("Leitura ");
	    Serial.print(contSensorReads);
	    Serial.print(": ");
	    Serial.println(temperatures[contSensorReads]);
		contSensorReads++;

		if(contSensorReads == TIMESTOUPLOADVALUE){
			// Reseta a variável contadora
			contSensorReads = 0;

			// Busca e formata a data e o horário atual
			String stringDatetime = getDatetime();

			// Realiza o tratamento estatístico para obter a temperatura correspondente
			int SensorMedian = vectorMedian();
			
			// Temperatura correspondente é colocada no banco de dados.
			// O nome da variável está no formato "data/horário"
			Firebase.setFloat(stringDatetime, SensorMedian);
			// Lida com algum possível erro
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
				Serial.println(String(SensorMedian));
				Serial.println(EMPTYSPACE);
			}
		}
	}
}
