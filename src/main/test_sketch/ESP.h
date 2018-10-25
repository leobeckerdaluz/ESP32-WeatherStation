// SSID do WiFi
#define WIFI_SSID "Estrela-32"
// PASSWORD do WiFi
#define WIFI_PASSWORD "AdC111729"
// Firebase Database URL
#define databaseURL "engenharia-de-software-fa3bf.firebaseio.com"
// Definição do caminho onde os dados do sensor serão armazenados
#define LOGSPATH "logs/"

// Pino onde o Sensor está conectado
#define SENSORPIN 33
// Pino onde o Atuador está conectado
#define ACTUATORPIN 23
// Pino onde o LED de status está conectado
#define STATUSPIN 2
// Definição de quantas vezes o valor do sensor será lido até que haja o tratamento (Máx: 255)
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

//------------------------------------------------------

// Definições para a data e horário
// const char* ntpServer = "pool.ntp.org";
// const long gmtOffset_sec = -10800;
// const int daylightOffset_sec = 0;

//------------------------------------------------------

extern void setTempOnFirebase(String stringDatetime, float tempCelcius);
extern float ADCtoTemperature(int adc_read);
extern float getArrayMedian(float v_array[], int array_size);
extern String getDatetime(tm timeinfo);