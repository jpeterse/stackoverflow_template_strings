#pragma once
#ifndef SEARCH_TEMPLATE_T
  #define SEARCH_TEMPLATE_T

  #include <Arduino.h>

  namespace SEARCH_TMPL {
    void begin();
    void reset();
    void set_search_chr( char c );
    bool hasTemplate( char* line );
    bool find_key( char* line, size_t &pos );
    size_t getMatchStart();
    size_t getMatchEnd();
    size_t getMatchLength();
    size_t getVariableStart();
    size_t getVariableEnd();
    size_t getVariableLength();
  }

#endif