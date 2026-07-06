#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "IBM_BIOS_Font.h"

#include "langtons_ant.c"

#define BLK_PIN 11 // Pin for backlight control, if used. Set to -1 if not used
#define ASCII_SIZE 1

#define ASCII_WIDTH (TFT_WIDTH/8)/ASCII_SIZE
#define ASCII_HEIGHT (TFT_HEIGHT/8)/ASCII_SIZE

void draw_ascii_buffer();

char ascii_pallete[] = ".,;:!^*+%#@";
uint32_t ascii_pallete_colors[] = {
    0x000000, // Black
    0x1C1C1C, // Dark Gray
    0x383838, // Gray
    0x545454, // Light Gray
    0x707070, // Lighter Gray
    0x8C8C8C, // Even Lighter Gray
    0xA8A8A8, // Very Light Gray
    0xC4C4C4, // Almost White
    0xE0E0E0, // Near White
    0xFFFFFF, // White
};
uint8_t ascii_buffer[ASCII_WIDTH*ASCII_HEIGHT];
uint8_t old_ascii_buffer[ASCII_WIDTH*ASCII_HEIGHT];

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

void setup() {
  Serial.begin(115200);
  tft.init();
  pinMode(BLK_PIN, OUTPUT);
  analogWrite(BLK_PIN, 100); // Turn on backlight
  tft.fillScreen(TFT_BLACK);

  tft.setFreeFont(&ibm_bios_font);
  tft.setTextSize(ASCII_SIZE);
  memset(ascii_buffer, 0, sizeof(ascii_buffer));
  memset(old_ascii_buffer, 255, sizeof(old_ascii_buffer));
}

void loop() {
  static int last_report = 0;
  static int last_ascii_draw = 0;
  static int draw_ms = 0;
  static int last_ant_move = 0;

  if(millis() - last_report > 1000) {
    Serial.println("Draw time: " + String(draw_ms) + " ms");
    last_report = millis();
  }

  if(millis() - last_ant_move > 2){
    langtans_ant_process(ascii_buffer, ASCII_WIDTH, ASCII_HEIGHT, "LLRR");
    last_ant_move = millis();
  }
  
  if(millis() - last_ascii_draw > 100){
    int draw_start = millis();
    draw_ascii_buffer();
    draw_ms = millis() - draw_start;
    last_ascii_draw = millis();
  }
}

void draw_ascii_buffer(){
  for(int i = 0; i < ASCII_WIDTH; i++) {
    for(int j = 0; j < ASCII_HEIGHT; j++) {
      if(ascii_buffer[i * ASCII_HEIGHT + j] == old_ascii_buffer[i * ASCII_HEIGHT + j]) continue;
      tft.fillRect(i * 8 * ASCII_SIZE, j * 8 * ASCII_SIZE, 8 * ASCII_SIZE, 8 * ASCII_SIZE, TFT_BLACK);
      
      String chr = String(ascii_pallete[ascii_buffer[i * ASCII_HEIGHT + j]]);
      tft.setTextColor(ascii_pallete_colors[ascii_buffer[i * ASCII_HEIGHT + j]]);
      tft.drawString(chr, i * 8 * ASCII_SIZE, j * 8 * ASCII_SIZE);
    }
  }
  memcpy(old_ascii_buffer, ascii_buffer, sizeof(ascii_buffer));
}