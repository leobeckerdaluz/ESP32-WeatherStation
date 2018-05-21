#include <IOXhop_FirebaseESP32.h>
#include <WiFi.h>

// SSID do WiFi
#define WIFI_SSID "Cagghetto"
// PASSWORD do WiFi
#define WIFI_PASSWORD "12345679"
// Firebase Database URL
#define databaseURL "engenharia-de-software-fa3bf.firebaseio.com"

void setup() {
	// Pino 2 onde o atuador está conectado é declarado como saída
	pinMode(2, OUTPUT);
	
	// Comunicação Serial é inicializada com Baud_rate 9600
	Serial.begin(9600);
	
	// Conexão com WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("connecting");

	// Enquanto o WiFi não está conectado, tenta conectar a cada 300ms
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(300);
	}
	Serial.print("connected: ");
	Serial.println(WiFi.localIP());
	Serial.println("");
	
	// Firebase é incializado
	Firebase.begin(databaseURL);

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
				digitalWrite(2, HIGH);
			}else if(statusActuator == "false"){
				digitalWrite(2, LOW);
			}
			// String path = stream.getPath();
			// String data = stream.getDataString();
		}
	});  
}

void loop() {
	// na pasta logs, um valor é inserido a cada 2 segundos no database
	// String é formatada em relação ao valor de "N"
	// Exemplo: logs/number1...logs/number2...logs/number3
	// Juntamente com esse nome, o valor de N é adicionado ao database
	String upload = "logs/number";
	Serial.println(upload);
	upload += n;
	Serial.println(upload);
	// N é atualizado
	n = n+1;

	// Valor de N é adicionado ao database juntamente com seu nome
	Firebase.setFloat(upload, n);
	// Lida com algum possível erro
	if (Firebase.failed()) {
		Serial.print("setting /number failed:");
		Serial.println(Firebase.error());  
		return;
	}

	// Delay de 2 segundos é realizado
	delay(2000);
}
