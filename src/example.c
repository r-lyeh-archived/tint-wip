#include "log.h"
#include <stdlib.h>

int main(void) {
  srand( time(0) );
  for( int i = 0, end = rand() % 10; i < end; ++i ) printf("%s\n", "regular message");
  for( int i = 0, end = rand() % 10; i < end; ++i ) log_info("logging an info message");
  for( int i = 0, end = rand() % 10; i < end; ++i ) log_warn("logging a warn message");
  for( int i = 0, end = rand() % 10; i < end; ++i ) log_debug("logging a debug message");
  for( int i = 0, end = rand() % 10; i < end; ++i ) log_error("logging an error");
  return 0;
}
