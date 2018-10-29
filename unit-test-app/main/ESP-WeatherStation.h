#define DELAY_BLINK_ON 10
#define DELAY_BLINK_OFF 100
#define NUM_BLINKS_INIT 10
#define TIMESTOUPLOADVALUE 10
#define DELAY_EACH_ADC_READ 1000

/*WIFI*/
// #define EXAMPLE_WIFI_SSID "CS6"
#define EXAMPLE_WIFI_SSID "Estrela-32"
// #define EXAMPLE_WIFI_PASS "luisdaluz6"
#define EXAMPLE_WIFI_PASS "AdC111729"

/*ADC*/
#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

/*STATUSLED*/
#define STATUS_LED 2

extern esp_adc_cal_characteristics_t *adc_chars;
extern EventGroupHandle_t wifi_event_group;

/* TIME */
extern time_t now;
extern struct tm timeinfo;

extern void init_time(void);
extern void init_adc(void);
extern void init_spiffs(void);
extern uint32_t voltageADCreading();
extern float milivoltsTOcelciusLM35(uint32_t voltage);
extern float getArrayMedian(float v_array[], int array_size);
extern void obtain_time(void);
extern esp_err_t event_handler(void *ctx, system_event_t *event);
extern void initialise_wifi(void);
