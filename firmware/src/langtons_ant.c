#include <stdint.h>

void langtans_ant_process(uint8_t* buffer, int width, int height, const char* rules) {
  static int ant_x = 15;
  static int ant_y = 15;
  static int ant_dir = 0; // 0=up, 1=right, 2=down, 3=left
  // Get the current pixel color
  int index = ant_y * width + ant_x;
  uint8_t current_color = buffer[index];


  // Update the ant's direction
  if (rules[current_color] == 'R') {
    ant_dir = (ant_dir + 1) % 4; // Turn right
  } else {
    ant_dir = (ant_dir + 3) % 4; // Turn left
  }

  // Flip the color of the current pixel
  buffer[index] = (current_color + 1) % strlen(rules);

  // Move the ant forward
  switch (ant_dir) {
    case 0: ant_y = (ant_y - 1 + height) % height; break; // Up
    case 1: ant_x = (ant_x + 1) % width; break; // Right
    case 2: ant_y = (ant_y + 1) % height; break; // Down
    case 3: ant_x = (ant_x - 1 + width) % width; break; // Left
  }
}