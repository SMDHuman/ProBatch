#ifndef ASCII_PROCEDURE_H
#define ASCII_PROCEDURE_H

#include <Arduino.h>

class ASCII_Procedure {
  public:
    uint32_t ascii_palette_colors[256];
    size_t width, height;
    bool vibrate_flag;
    int fps = 10;

    virtual ~ASCII_Procedure() = default;
    virtual void update(char *buffer) {}
    virtual void setup() {}
    void vibrate(){
      this->vibrate_flag = true;
    }
    void map_colors_to_chars(uint32_t* palette_rgb, const char* ascii_palette){
      const uint8_t palette_len = (uint8_t)strlen(ascii_palette);
      for (uint8_t i = 0; i < palette_len; i++) {
        this->ascii_palette_colors[(uint8_t)ascii_palette[i]] = palette_rgb[i];
      }
    }
};

#endif // ASCII_PROCEDURE_H