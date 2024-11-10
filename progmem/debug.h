#pragma once
#ifndef DEBUG_H
  #define DEBUG_H

  /* as defined in https://arduino-esp8266.readthedocs.io/en/latest/Troubleshooting/debugging.html 
  /  use the build in define statements for debugging, rather than the defines below, whenever possible.
  /  CUstom debug messages can be issued with macro DEBUG_NSG(); which is a printf invocation.

  / to enable debug messages, go to tool menu and set "Debug port" to Serial.
  / Common format specifiers:
  /   hh - signed char
  /   d or i – signed decimal integer
  /   u – unsigned decimal integer
  /   s – a string of characters
  /   h - short integer
  /   z - size_t
  /   l - long integer
  /   ll - long long integer
  /   L - long double
  */

  #ifdef DEBUG_ESP_PORT
    #define DEBUG_MSG(...) Serial.print( "[" ); Serial.print( __FUNCTION__ ); Serial.print(":"); Serial.print( __LINE__ ); Serial.print( "] "); Serial.printf( __VA_ARGS__ ); Serial.flush();
    #else
    #define DEBUG_MSG(...)
  #endif

#endif