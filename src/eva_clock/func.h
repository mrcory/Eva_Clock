//Extra Functions

CRGB HOLD_ARRAY[50];

void REVERSE_ARRAY2(int _ORIG_START, int _ORIG_NUM) {
  for (int i = 0 ; i < _ORIG_NUM ; i++) {
    HOLD_ARRAY[i] = leds2[i + _ORIG_START];
  }

  for (int i = 0 ; i < _ORIG_NUM ; i++) {
     leds2[_ORIG_NUM - i] = HOLD_ARRAY[i];
  }
} 
