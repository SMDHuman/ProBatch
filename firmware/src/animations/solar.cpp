#include <stdint.h>
#include <string.h>
#include "ascii_procedure.h"

class Solar_Pro : public ASCII_Procedure {
  //===================================
  private:
    int fps = 4;
    float sun_size = 3.6;
    float moon_rad = 0;
    float moon_dist = 3.6;
    float moon_size = 0.8;
    float earth_rad = 3;
    float earth_dist = 10;
    float earth_size = 2;
  //===================================
  public:
    void setup(){
      ascii_palette_colors['@'] = 0xFFFF00; // Sun color
      ascii_palette_colors['O'] = 0x888888; // Moon color
      ascii_palette_colors['o'] = 0x0000FF; // Earth sea color
      ascii_palette_colors['e'] = 0x00FF00; // Earth grass color
    }
  //===================================
    void update(char* buffer){

      earth_rad += 0.05f; // Adjust the speed of Earth's orbit
      moon_rad += 0.4f; // Adjust the speed of Moon's orbit

      float sun_x = 0;
      float sun_y = 0;
      float earth_x = cos(earth_rad) * earth_dist;
      float earth_y = sin(earth_rad) * earth_dist;
      float moon_x = earth_x + cos(moon_rad) * moon_dist;
      float moon_y = earth_y + sin(moon_rad) * moon_dist;

      bool moon_drawn = 0;
      for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
          float dx = (float)x - (float)this->width / 2.0f;
          float dy = (float)y - (float)this->height / 2.0f;
          if(sq(dx - sun_x) + sq(dy - sun_y) < sq(sun_size)) {
            buffer[y * this->width + x] = '@'; // Sun
          } else if(!moon_drawn && sq(dx - moon_x) + sq(dy - moon_y) < sq(moon_size)) {
            moon_drawn = 1;
            buffer[y * this->width + x] = 'O'; // Moon
          } else if(sq(dx - earth_x) + sq(dy - earth_y) < sq(earth_size)) {
            if(random(0, 2) == 0) {
              buffer[y * this->width + x] = 'o'; // Earth sea
            } else {
              buffer[y * this->width + x] = 'e'; // Earth grass
            }
          } else {
            buffer[y * this->width + x] = ' '; // Empty space
          }
        }
      }
    }
};
