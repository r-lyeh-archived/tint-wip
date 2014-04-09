#pragma once

enum LogColor {
  COLOR_DEFAULT,
  COLOR_NONE, //BLACK
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_MAGENTA,
  COLOR_CYAN,
  COLOR_WHITE
};

void ColorPrintf(int color, const char* fmt, ...);
