/*
 * TO DO:
 * add logic to break out numbers to text equivalents
 * add logic to handle spaces with a pause
 * add logic to drop special characters
 */


#include "FastLED.h"
#define NUM_LEDS 26
#define PIN 13
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <WString.h>

ESP8266WiFiMulti WiFiMulti;
CRGB leds[NUM_LEDS];

//modify this line with the website where index.php is hosted
const char messageHost[] = "http://domain.com/?mode=getmessage";

//function to blink status lights while connecting to wifi
void statusBlink() {
  for(int x = 1; x < 3; x++) {
    leds[11] = CRGB::Blue;
    leds[16] = CRGB::Blue;
    leds[17] = CRGB::Blue;
    leds[18] = CRGB::Blue;
    leds[24] = CRGB::Blue;
    FastLED.show();
    delay(100);
    leds[11] = CRGB::Black;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[24] = CRGB::Black;
    FastLED.show();
    delay(100);
  }  
}

void setup() {
  //limit power and initialize LED string
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500); 
  FastLED.addLeds<WS2811 , PIN>(leds, NUM_LEDS);
  
  //Serial.begin(115200);
  
  // We start by connecting to a WiFi network
  WiFiMulti.addAP("SSID", "password");
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
      statusBlink();
    }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //for OTA updates
  ArduinoOTA.setHostname("strangerWall");
  //ArduinoOTA.setPassword((const char *)"123");
  ArduinoOTA.begin();
  
}

void loop() {
  //msg[] is the max message length, feel free to modify it
  char msg[140];
  String payload;   
  // wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {
    ArduinoOTA.handle();
    HTTPClient http;
    //Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(messageHost); //HTTP

    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == HTTP_CODE_OK) {
        payload = http.getString();
        //Serial.print("payload=");
        //Serial.println(payload);
        //Serial.print("payload length=");
        //Serial.println(payload.length());
        payload.toCharArray(msg, 140);
      }
    } else {
      
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      strcpy(msg, "error");
      payload = "error";
    }
      http.end();
      
  }

  
  Serial.print("msg=");
  Serial.println(msg);
  //Serial.print("Sizeof msg=");
  //Serial.println(sizeof(msg));
  //Serial.println("--begin display message--");
  int x=0;
  for(x = 0; x <= (payload.length() - 1) ; x++) {
    //random returns value between min and max-1
    //randomize color, delay and brightness:
    int rndColor = random(1,256);
    int ledDelay = random(1000, 1500);
    int bright = random(15, 75);
  
    //placeholder for letter to be selected
    int letterLED;
    //determine the letter to light up
    //numbering is not linear because string weaves back and forth
    switch (msg[x]) {
      case 'a':
        letterLED = 0;
        break;
      case 'b':
        letterLED = 1;
        break;
      case 'c':
        letterLED = 2;
        break;
      case 'd':
        letterLED = 3;
        break;
      case 'e':
        letterLED = 4;
        break;
      case 'f':
        letterLED = 5;
        break;
      case 'g':
        letterLED = 6;
        break;
      case 'h':
        letterLED = 7;
        break;
      case 'o':
        letterLED = 8;
        break;
      case 'n':
        letterLED = 9;
        break;
      case 'm':
        letterLED = 10;
        break;
      case 'l':
        letterLED = 11;
        break;
      case 'k':
        letterLED = 12;
        break;
      case 'j':
        letterLED = 13;
        break;
      case 'i':
        letterLED = 14;
        break;
      case 'p':
        letterLED = 15;
        break;
      case 'q':
        letterLED = 16;
        break;
      case 'r':
        letterLED = 17;
        break;
      case 's':
        letterLED = 18;
        break;
      case 't':
        letterLED = 19;
        break;
      case 'u':
        letterLED = 20;
        break;
      case 'z':
        letterLED = 21;
        break;
      case 'y':
        letterLED = 22;
        break;
      case 'x':
        letterLED = 23;
        break;
      case 'w':
        letterLED = 24;
        break;
      case 'v':
        letterLED = 25;
        break;
    }
    //set hue,saturation, brightness
    leds[letterLED] = CHSV(rndColor, 255, bright);
    
    //Serial.print("light up now=");
    //Serial.println(msg[x]);
    //light the letter
    FastLED.show();

    //keep it lit for the delay
    delay(ledDelay);
    
    
    //turn it off
    //Serial.print("turn off now=");
    //Serial.println(msg[x]);
    leds[letterLED] = CRGB::Black;
    FastLED.show();
    //delay(50);
  }
  
  //Serial.println("--end display message & 3 second pause--");
  //message has been displayed. need to pause and/or indicate completion
  delay(3000);
  //statusBlink(3);
  //Serial.println("-----end main loop-----");
}
