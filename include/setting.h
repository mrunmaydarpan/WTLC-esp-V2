/*------------sensor setting-----------------*/

// #define SENSOR_1 true // sonar
#define SENSOR_2 true // UART
// #define SENSOR_3 true // PWM

/*------------wifi setting-------------------*/

#define AP_MODE true // AP Mode
// #define WM_SET  // (wifimanager) this turns off the wifi feature

/*------------API setting--------------------*/
#if WM_SET
#define HA_INIT false // if using with home assistant {turn on WM_SET to enable this feature}
#if HA_INIT
#define BROKER_ADDR IPAddress(192, 168, 1, 6) // home assistant IP
#endif
#endif
/*-------------------------------------------*/

#define SW_TEST true
#define OLED true // use OLED display
// #define DryRun //dryrun feature
#define debugData false
#define Buzzer true
// #define web_setting //use this to control without phone

#define on_delay 500
#define off_delay 1000

#if SENSOR_1
#define SENSOR_DISP 1
#elif SENSOR_2
#define SENSOR_DISP 2
#elif SENSOR_3
#define SENSOR_DISP 3
#endif

#ifdef DryRun
#define DryRunTime 120000 // 120sec
#endif