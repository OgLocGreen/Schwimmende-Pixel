#include <Arduino.h>
#include "WiFi.h"

#include "Adafruit_NeoPixel.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/// Defines for OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    false // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/// Defindes for Webservice
const char* ssid = "FRITZ!Box 7590 VO";
const char* password = "91272756878874074534";

//Defines for RGB-Ring
#define PIN 15
#define LEDS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GBR + NEO_KHZ800);

void setup(){
  Serial.begin(9600);


 //Anzeigen von IP und Akku auf OLed Display
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(F("Hello"));

  strip.begin();
  strip.setBrightness(50);
  for(int i=0; i<strip.numPixels()+1; i++) {
    strip.setPixelColor(i, strip.Color(125, 125, 125));
    strip.show();
    delay(1);
    }

  WiFi.begin(ssid, password);
 /// Verbinden von WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  /// Verbinden von Oled Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
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
  

}
 
void loop(){
  delay(1);
}
