//Enable Home Assistant Integration
#define ENABLE_HOME_ASSISTANT

//Home Assistant Config
#define UNIQUE_NAME "Eva_Clock_00"
#define BROKER_ADDR      IPAddress(192,168,254,84)
#define BROKER_USER      ""
#define BROKER_PASS      ""

#define VERSION "0.0.1"

// Unique ID must be set!
byte mac[] = {0x87, 0x21, 0xE1, 0x7F, 0xFA, 0xF7};

//LED Strip Data Pins
#define pin_INNER  22
#define pin_OUTER  16
#define pin_STATUS 21

//Naming
#define MY_HOST_NAME "Eva Clock"

//Enable NTP
#define NTP_ENABLE

//UTC Offset in seconds
long TIME_OFFSET = -14400;

//Set to 12 hour clock
#define HOUR_12
//#define HOUR_24

//Tweakable Values
int HUE_SPEED = 400;

//Color Scheme
//Clock Characters
const CHSV C_7_SEGMENT            = CHSV(38,255,150);
const CHSV C_COLONS               = CHSV(38,255,150);
//All Text
const CHSV C_GENERAL_TEXT         = CHSV(38,255,150);
//Background Hue
const CHSV C_BACKGROUND_HUE_START = CHSV(148,255,255);
const CHSV C_BACKGROUND_HUE_END   = CHSV(0,255,255);
//Power Status LEDs
const CHSV C_POWER_STATUS_EXT     = CHSV(85,255,150);
const CHSV C_POWER_STATUS_INT     = CHSV(0,255,150);
//Lower Status Lights
const CHSV C_CURRENT_STATUS       = CHSV(0,255,250);

//Countdown Colors




/*LED Positions in the below order   
 *       7
 *     6   5
 *       4
 *     3   2
 *       1
 */
const byte DIGITS [6] [7] = {
  {11, 13, 12, 31, 33, 32, 49}, //Hour 1
  {10, 15, 14, 30, 35, 34, 48}, //Hour 2
  {9, 17, 16, 28, 37, 36, 47},  //Minute 1
  {8, 19, 18, 27, 39, 38, 46},  //Minute 2
  {7, 21, 20, 25, 41, 40, 45},  //Second 1
  {6, 23, 22, 24, 43, 42, 44}   //Second 2
};

//Power leds
const byte INTERNAL_TEXT_LED = 0;
const byte EXTERNAL_TEXT_LED = 4;

//Colons that divide hour and minute
const byte COLONS   [2] = {26, 29};

const byte TEXT_LEFT [] = {50,51,52};

//Extras
long COUNTDOWN_MILLIS = 5000;//15000;//300000; 
