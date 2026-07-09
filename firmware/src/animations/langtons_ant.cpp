#include <stdint.h>
#include <string.h>
#include "ascii_procedure.h"

class Langtons_Pro : public ASCII_Procedure {
  //===================================
  private:
    const char ascii_palette[16] = " .,;:!\\/()<>&#@"; // Example ASCII palette
    const char rules[3][16] = {"LLRRLLRR\0", // Rule sequence for cyclic palette states
                                "RRLLRRLL\0",
                                "LRRRRRLLR\0"};
    int rule_index = 0; // Index to select which rule to use
    int ant_x = 15;
    int ant_y = 15;
    int speed = 200; // Number of steps per second
    int ant_dir = 1; // 0=up, 1=right, 2=down, 3=left
    int fps = 10;
  //===================================
  public:
    void setup(){
      // random rgb
      uint32_t palette_rgb[16];
      for (int i = 0; i < 16; i++) {
        int r = random(0, 256);
        int g = random(0, 256);
        int b = random(0, 256);
        palette_rgb[i] = (r << 16) | (g << 8) | b;
      }
      this->map_colors_to_chars(palette_rgb, this->ascii_palette);
      ant_x = this->width / 2;
      ant_y = this->height / 2;
    }
  //===================================
    void update(char* buffer){
      if(this->vibrate_flag) {
        this->vibrate_flag = false;
        rule_index = (rule_index + 1) % (sizeof(this->rules) / sizeof(this->rules[0])); // Switch between the rules
        memset(buffer, ascii_palette[0], this->width * this->height); // Reset the buffer to the first color
        ant_x = this->width / 2;
        ant_y = this->height / 2;
      }
      for(int i = 0; i < this->speed/this->fps; i++) {
        this->step(buffer);
      }
      // buffer[ant_y * this->width + ant_x] = this->ascii_palette[14]; // Mark the ant's position with the first color
    }
    void step(char* buffer){
      const uint8_t rules_len = (uint8_t)strlen(this->rules[rule_index]);
      int index = ant_y * this->width + ant_x;
      char current_color = buffer[index];
      uint8_t ascii_index = 0;
      for (uint8_t i = 0; i < rules_len; i++) {
        if (ascii_palette[i] == current_color) {
          ascii_index = i;
          break;
        }
      }
  
      // Update the ant's direction
      if (this->rules[rule_index][ascii_index % rules_len] == 'R') {
        ant_dir = (ant_dir + 1) % 4; // Turn right
      } else {
        ant_dir = (ant_dir + 3) % 4; // Turn left
      }

      // Flip the color of the current pixel
      buffer[index] = this->ascii_palette[(ascii_index + 1) % rules_len];

      // Move the ant forward
      switch (ant_dir) {
        case 0: ant_y = (ant_y - 1 + this->height) % this->height; break; // Up
        case 1: ant_x = (ant_x + 1) % this->width; break; // Right
        case 2: ant_y = (ant_y + 1) % this->height; break; // Down
        case 3: ant_x = (ant_x - 1 + this->width) % this->width; break; // Left
      }
    }
};
