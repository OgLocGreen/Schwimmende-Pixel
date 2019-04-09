#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Adafruit_NeoPixel.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPIFFSEditor.h>
#include <ArduinoOTA.h>
#include <FS.h>

/// Defines for OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    false // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Defines for RGB-Ring
#define PIN 18
#define LEDS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GBR + NEO_KHZ800);
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);

/// Defindes for Webservice
const char* ssid = "da34-2,4";
const char* password = "arschlochkind4";
/*
//Hochschul Wlan
const char* ssid = "FRITZ!Box 7590 VO";
const char* password = "91272756878874074534";
*/
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
//AsyncEventSource events("/events");

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);



void setup(){
  Serial.begin(9600);
  Serial.println("Hello my Friend, starting ESP32 ");
  /// Verbinden von Oled Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //Anzeigen von IP und Akku auf OLed Display
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(F("Hello my Friend, starting ESP32"));
  display.display();


  //Beginn LED-Ring
  strip.begin();
  strip.setBrightness(50);
  rainbow(20);

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  }
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
      }
  }

  // Verbinden von WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {   // Solange versuchen zu verbinden bis erfolg
    int i = 0;
    // Some example procedures showing how to display to the pixels:
    if(i%2==0)
    {
      colorWipe(strip.Color(255, 0, 0), 100); // Red
    }else if (i%2==1)
    {
      colorWipe(strip.Color(0, 0, 255), 100); // Blue
    }else
    {
      colorWipe(strip.Color(0, 255, 0), 100); // Green
    }
    delay(10000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());   //Anzeigen von Ip über Serial
  display.clearDisplay();             //Anzeigen von IP und Akku auf OLed Display
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(F("IP:"));
  display.println(WiFi.localIP());
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,20);             
  display.println(F("Akku:"));
  display.print("AKKU");
  display.display();
  delay(2000);

  
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  //server.addHandler(&events);

  server.begin();



}
   
void loop(){

}


void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      }
      else{
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());
      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    }
    else {        ////////////wird glaub nicht gebraucht
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


// Some Funktions for the LED-Ring
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}