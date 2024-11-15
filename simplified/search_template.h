#pragma once
#ifndef SEARCH_TEMPLATE_H
  #define SEARCH_TEMPLATE_H

  #include <Arduino.h>

  namespace SEARCH_TMPL {
    void begin();
    void reset();
    void set_search_chr( char c );
    bool hasTemplate( char* line );
    bool find_key( char* line, size_t &pos );
    bool replace_str( char* line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size );
    bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t replace_length );
    size_t getMatchStart();
    size_t getMatchEnd();
    size_t getMatchLength();
    size_t getVariableStart();
    size_t getVariableEnd();
    size_t getVariableLength();
  }

#endif