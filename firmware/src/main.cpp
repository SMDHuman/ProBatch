#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "IBM_BIOS_Font.h"

#define ASCII_PROCEDURE_IMPLEMENTATION
#include "animations/ascii_procedure.h"
#include "animations/langtons_ant.cpp"
#include "animations/solar.cpp"

#define BLK_PIN -1 // Pin for backlight control, if used. Set to -1 if not used
#define VIB_PIN 11
#define VIB_VCC_PIN 10
#define ASCII_SIZE 1

#define ASCII_WIDTH (TFT_WIDTH/8)/ASCII_SIZE
#define ASCII_HEIGHT (TFT_HEIGHT/8)/ASCII_SIZE

void draw_ascii_buffer();

char ascii_buffer[ASCII_WIDTH*ASCII_HEIGHT];
char old_ascii_buffer[ASCII_WIDTH*ASCII_HEIGHT];

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

Langtons_Pro langton_pro;
Solar_Pro solar_pro;
ASCII_Procedure* pro_list[2] = { &langton_pro, &solar_pro };
int active_pro_index = 1;
ASCII_Procedure* active_pro = pro_list[active_pro_index];

//-----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  //...
  if(BLK_PIN != -1) {
    pinMode(BLK_PIN, OUTPUT);
    analogWrite(BLK_PIN, 100); // Turn on backlight
  }
  pinMode(VIB_PIN, INPUT_PULLDOWN);
  pinMode(VIB_VCC_PIN, OUTPUT);
  digitalWrite(VIB_VCC_PIN, HIGH); // Power the vibration sensor
  //...
  tft.init();
  tft.fillScreen(TFT_BLACK);
  //...
  tft.setFreeFont(&ibm_bios_font);
  tft.setTextSize(ASCII_SIZE);
  memset(ascii_buffer, ' ', sizeof(ascii_buffer));
  memset(old_ascii_buffer, 255, sizeof(old_ascii_buffer));
  //...
  for (size_t i = 0; i < sizeof(pro_list)/sizeof(pro_list[0]); i++) {
    pro_list[i]->width = ASCII_WIDTH;
    pro_list[i]->height = ASCII_HEIGHT;
    pro_list[i]->setup();
  }
}

//-----------------------------------------------------------------------------
void loop() {
  static int last_report = 0;
  static int last_ascii_draw = 0;
  static int draw_ms = 0;
  static int last_vibrate = 0;
  static int last_action = 0;
  //...
  if(digitalRead(VIB_PIN) && millis() - last_vibrate > 350) {
    active_pro_index = (active_pro_index+1) % (sizeof(pro_list)/sizeof(pro_list[0]));
    active_pro = pro_list[active_pro_index];
    tft.fillScreen(TFT_BLACK);
    memset(ascii_buffer, ' ', sizeof(ascii_buffer));
    active_pro->setup();
    Serial.println("Vibration detected, switching animation.");
    last_vibrate = millis();
  }
  //...
  if(millis() - last_report > 1000) {
    Serial.println("Draw time: " + String(draw_ms) + " ms");
    last_report = millis();
  }
  //...
  if(millis() - last_ascii_draw > 1000 / active_pro->fps) {
    //...
    active_pro->update((char*)ascii_buffer);
    //...
    int draw_start = millis();
    draw_ascii_buffer();
    draw_ms = millis() - draw_start;
    last_ascii_draw = millis();
  }
}

//-----------------------------------------------------------------------------
void draw_ascii_buffer(){
  for(int i = 0; i < ASCII_WIDTH; i++) {
    for(int j = 0; j < ASCII_HEIGHT; j++) {
      int idx = j * ASCII_WIDTH + i;
      if(ascii_buffer[idx] == old_ascii_buffer[idx]) continue;
      tft.fillRect(i * 8 * ASCII_SIZE, j * 8 * ASCII_SIZE, 8 * ASCII_SIZE, 8 * ASCII_SIZE, TFT_BLACK);
      
      String chr = String(ascii_buffer[idx]);
      uint32_t color24 = active_pro->ascii_palette_colors[(uint8_t)ascii_buffer[idx]];
      uint16_t color = tft.color565(color24 >> 16, (color24 >> 8) & 0xFF, color24 & 0xFF);
      tft.setTextColor(color);
      tft.drawString(chr, i * 8 * ASCII_SIZE, j * 8 * ASCII_SIZE);
    }
  }
  memcpy(old_ascii_buffer, ascii_buffer, sizeof(ascii_buffer));
}