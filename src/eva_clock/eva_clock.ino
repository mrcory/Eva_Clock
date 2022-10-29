#include <ArduinoOTA.h>
#include <FastLED.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Ethernet2.h>
#include <mDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


#include "struct.h"
#include "universalFunc.h"
#include "config.h"

#ifdef ENABLE_HOME_ASSISTANT
  #include <ArduinoHA.h>
#endif


#define ledType WS2812
#define colorOrder GRB
#define NUM_LEDS  52 //91
#define NUM_LEDS2 44  //Outer
#define NUM_LEDS3 8   //Status Lights

CRGB leds[NUM_LEDS];   //Digits and Text
CRGB leds2[NUM_LEDS2]; //Background
CRGB leds3[NUM_LEDS3]; //Lower Status light

#include "func.h"


#ifdef NTP_ENABLE
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, TIME_OFFSET);
#endif

//Copied variabls
int brightness = 30;
int originalBrightness = 8;
int oldBrightness = brightness;
int FPS = 60;

#include "clock.h"

struct LightValues {
    bool lightOn;
    int brightness;
  };

  LightValues lightValues = { .lightOn = true, .brightness = brightness };

#ifdef ENABLE_HOME_ASSISTANT
  WiFiClient client;
  
  HADevice device(mac, sizeof(mac));
  HAMqtt mqtt(client, device);

  HALight light(UNIQUE_NAME, HALight::BrightnessFeature);


  void onStateCommand(bool state, HALight* sender) {
    Serial.print("State: ");
    Serial.println(state);

    if (state == false) {
      oldBrightness = lightValues.brightness;
      lightValues.brightness = 0;
    }

    if (state == true && lightValues.brightness == 0) {
      lightValues.brightness = oldBrightness;
    }

    lightValues.lightOn = state;
    
    sender->setState(state); // report state back to the Home Assistant
}

void onBrightnessCommand(uint8_t brightness, HALight* sender) {
    Serial.print("Brightness: ");
    Serial.println(brightness);

    lightValues.brightness = brightness;
    lightValues.lightOn = true;


    sender->setBrightness(brightness); // report brightness back to the Home Assistant
}
  
#endif

void setup() {
  WiFiManager wifi;

#ifdef ENABLE_HOME_ASSISTANT
  device.setName(MY_HOST_NAME);
  device.setSoftwareVersion(VERSION);

  light.setState(true); //Light starts on, so let us tell HA

  light.setName(MY_HOST_NAME);


  light.onStateCommand(onStateCommand);
  light.onBrightnessCommand(onBrightnessCommand);

  mqtt.begin(BROKER_ADDR, BROKER_USER, BROKER_PASS);

  WiFi.macAddress(mac);
#endif
  
  
  wifi.autoConnect(MY_HOST_NAME);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
  Serial.begin(115200);

    
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.setPassword("admin");
  ArduinoOTA.setHostname(MY_HOST_NAME);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  #ifdef NTP_ENABLE
    timeClient.begin();
    timeClient.setUpdateInterval(60000);
  #endif

    
  Serial.begin(115200);

  FastLED.addLeds<ledType,pin_INNER,colorOrder>(leds,0,NUM_LEDS);
  FastLED.addLeds<ledType,pin_OUTER,colorOrder>(leds2,0,NUM_LEDS2);
  FastLED.addLeds<ledType,pin_STATUS,colorOrder>(leds3,0,NUM_LEDS3);

  FastLED.setBrightness(brightness);

  timerSetup();
}

#ifdef ENABLE_HOME_ASSISTANT
  #include "ha.h"
#endif

void loop() {
  ArduinoOTA.handle();
  #ifdef ENABLE_HOME_ASSISTANT
    if (timer(500,2)) {
      mqtt.loop();
    }
  #endif
  
  #ifdef NTP_ENABLE
    timeClient.update();
  #endif

  if (timer(24,0)) {
    if (TIMER_RUNNING) {
      _tempCount = TIMER_END - millis();
      COUNTDOWN_TIME();
      STATUS_LIGHTS();
    } else {
      DISPLAY_TIME();
      STATUS_LIGHTS();
    }
  }

  if (timer(100,5)) {
    CONVERT_TIME();
  }

  if (timer(HUE_SPEED,1)) {
    RAINBOW_HUE++;
  }

  FastLED.setBrightness(lightValues.brightness);
  FastLED.show();
  FastLED.delay(1000/FPS);
}
