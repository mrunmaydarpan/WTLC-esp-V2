#define _VERSION "2.1"
#define _HARDWARE "9.0"

const char brand[] = "MDtronix";

/*------------sensor setting-----------------*/
// #define SENSOR_1 true // sonar
#define SENSOR_2 true // UART
// #define SENSOR_3 true // PWM
/*------------wifi setting-------------------*/
#define AP_MODE true // AP Mode
#define WM_SET true  // wifimanager

/*------------API setting--------------------*/
#define HA_INIT false // if using with home assistant
#if HA_INIT
#define BROKER_ADDR IPAddress(192, 168, 1, 6) // home assistant IP
#endif
/*-------------------------------------------*/

#define SW_TEST true
#define OLED true // use OLED display
#define DryRun
#define debugData false
#define Buzzer true
#define relayPin_inverted false
// #define web_setting

#define on_delay 500
#define off_delay 1000

#if WM_SET
#define WM_ASYNC
#endif
#if SENSOR_1
#define SENSOR_DISP 1
#elif SENSOR_2
#define SENSOR_DISP 2
#elif SENSOR_3
#define SENSOR_DISP 3
#endif