//Clock Functions

const CHSV COLOR_BLACK = CHSV(0,255,0);
const CHSV COLOR_RED   = CHSV(0,255,150);

bool MODE_COUNTDOWN = false;
bool HA_STATE = true;

//Define lower row "state" positions
const byte STATE_STOP   = 4;
const byte STATE_SLOW   = 5;
const byte STATE_NORMAL = 6;
const byte STATE_RACING = 7;

//Initial State
byte CURRENT_STATE = STATE_NORMAL;

//Bitmap for numbers
const byte DIGIT_ARRAY [] [7] = {
  // 0
  {1,1,1,0,1,1,1},
  // 1
  {0,1,0,0,1,0,0},
  //2
  {1,0,1,1,1,0,1},
  //3
  {1,1,0,1,1,0,1},
  //4
  {0,1,0,1,1,1,0},
  //5
  {1,1,0,1,0,1,1},
  //6
  {1,1,1,1,0,1,1},
  //7
  {0,1,0,0,1,0,1},
  //8
  {1,1,1,1,1,1,1},
  //9
  {1,1,0,1,1,1,1},
  //nil
  {0,0,0,0,0,0,0}
};

//Hold the time in an array
byte TIME_ARRAY [6] = {0};
byte COUNTDOWN_ARRAY [6] = {0};

int RAINBOW_HUE = 0;

#ifdef NTP_ENABLE
  void CONVERT_TIME() {

    #ifdef HOUR_12
    if (timeClient.getHours() > 12) {
      byte  _temp_time = timeClient.getHours() - 12;
      TIME_ARRAY [0] = _temp_time / 10;
      TIME_ARRAY [1] = _temp_time % 10;
    }
    #endif

    #ifdef HOUR_24
      TIME_ARRAY [0] = timeClient.getHours() / 10;
      TIME_ARRAY [1] = timeClient.getHours() % 10;
    #endif


    TIME_ARRAY [2] = timeClient.getMinutes() / 10;
    TIME_ARRAY [3] = timeClient.getMinutes() % 10;

    TIME_ARRAY [4] = timeClient.getSeconds() / 10;
    TIME_ARRAY [5] = timeClient.getSeconds() % 10;

    if (TIME_ARRAY[0] == 0) { //If HOUR 1 is 0, change it to nil display
      TIME_ARRAY[0] = 10;
    }
      
  }
#endif


void BLANK_DISPLAY() {
//Blank LEDs
  fill_solid(leds, NUM_LEDS, COLOR_BLACK);
  fill_solid(leds2, NUM_LEDS2, COLOR_BLACK);
  fill_solid(leds3, NUM_LEDS3, COLOR_BLACK);
}

//Display a number character to the selected position. (0-5) Number 0-9 and use 10 for nil display
void DISPLAY_NUMBER(byte _pos, byte _digit, CHSV _color = C_7_SEGMENT ) {
  for (int i = 0; i < 7; i++) {
    if (DIGIT_ARRAY[_digit][i] == 1) { //If segment should be lit, light it
      leds[DIGITS[_pos][i]] = _color;
    } else {
      leds[DIGITS[_pos][i]] = COLOR_BLACK; //If blank, turn off segment
    }
  }
}


void DISPLAY_TIME() {
  if (HA_STATE == true) {
    brightness == originalBrightness;
  } else {
    brightness = 0;
  }

  BLANK_DISPLAY(); //Blank LEDs at start
  
  for (int i=0; i<6; i++) { //Loop for each digit
    DISPLAY_NUMBER(i, TIME_ARRAY[i]);
  }

  //Light colons
  leds[COLONS[0]] = C_COLONS;
  leds[COLONS[1]] = C_COLONS;

  //Background
  fill_gradient(leds2, 18, C_BACKGROUND_HUE_START, 39, C_BACKGROUND_HUE_END, BACKWARD_HUES);
  fill_gradient(leds2, 0 , C_BACKGROUND_HUE_END, 17, C_BACKGROUND_HUE_START, LONGEST_HUES);

  for (int i = 40;i<44;i++) { //Fill extra top left LEDs
    leds2[i] = C_BACKGROUND_HUE_END;
  }


  leds[INTERNAL_TEXT_LED] = COLOR_BLACK;
  leds[EXTERNAL_TEXT_LED] = C_POWER_STATUS_EXT;
}

unsigned long TIMER_TRACKER [2] = {0,0};

bool TIMER_RUNNING = true;
unsigned long TIMER_START = millis();
unsigned long TIMER_END   = TIMER_START + COUNTDOWN_MILLIS;

unsigned long _tempCount = TIMER_END - millis();

void COUNTDOWN_TIME() {
  
  int _tempCountRemain = _tempCount;
  unsigned long _minutes  = (_tempCount / 60000);
  _tempCountRemain = _tempCount - (_minutes * 60000);
  unsigned long _seconds  = _tempCountRemain / 1000;
  _tempCountRemain -= (_seconds * 1000);
  unsigned long _millisec = _tempCountRemain / 10;

  COUNTDOWN_ARRAY[0] = _minutes / 10;
  COUNTDOWN_ARRAY[1] = _minutes % 10;
  COUNTDOWN_ARRAY[2] = _seconds / 10;
  COUNTDOWN_ARRAY[3] = _seconds % 10;
  COUNTDOWN_ARRAY[4] = _millisec / 10;
  COUNTDOWN_ARRAY[5] = _millisec % 10;
  
  if ((_minutes + _seconds) == 0) {
    TIMER_RUNNING = false;
  }
  
  for (int i=0; i<6; i++) { //Loop for each digit
    DISPLAY_NUMBER(i, COUNTDOWN_ARRAY[i], COLOR_RED);
  }

  for (int i = 0; i < NUM_LEDS2; i++) {
    leds2[i] =  COLOR_RED;
  }

  //Light colons
  leds[COLONS[0]] = COLOR_RED;
  leds[COLONS[1]] = COLOR_RED;

  leds[EXTERNAL_TEXT_LED] = COLOR_BLACK;
  leds[INTERNAL_TEXT_LED] = C_POWER_STATUS_INT;
}

void SET_RUN_STATE() {
  if (MODE_COUNTDOWN == true) {
    CURRENT_STATE = STATE_RACING;
  }
  
  if (MODE_COUNTDOWN == false) {
    CURRENT_STATE = STATE_NORMAL;
  }
}

void STATUS_LIGHTS() {
  SET_RUN_STATE();
  
  leds3[0] = C_GENERAL_TEXT; //Set text leds
  leds3[1] = C_GENERAL_TEXT; //Set text leds
  leds3[2] = C_GENERAL_TEXT; //Set text leds
  leds3[3] = C_GENERAL_TEXT; //Set text leds

  leds3[CURRENT_STATE] = C_CURRENT_STATUS; //Light up the current state
  

}
