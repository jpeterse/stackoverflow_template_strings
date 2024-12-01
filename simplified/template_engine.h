#pragma once
#ifndef SEARCH_TEMPLATE_H
  #define SEARCH_TEMPLATE_H

  #include <Arduino.h>

  namespace TMPL_ENGINE {
    void begin();
    void reset();
    void debug_print();
    void set_tmpl_delim_chr( char c );
    void set_attr_delim_chr( char c );
    bool hasTemplate( char* haystack );
    bool hasAttribute( char* haystack );
    bool find_key( char* haystack, char* delim, size_t &pos );
    bool find_operand( char* haystack, size_t &pos );
    bool replace_str( char* haystack, const char* new_val, uint16_t needle_start, uint16_t needle_end, uint16_t haystack_size );
    bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t new_val_length );
    size_t getMatchStart();
    size_t getMatchEnd();
    size_t getMatchLength();
    size_t getVariableStart();
    size_t getVariableEnd();
    size_t getVariableLength();
    size_t getAttributeStart();
    size_t getAttributeEnd();
    size_t getAttributeLength();
    size_t getOptionStart();
    size_t getOptionEnd();
    size_t getOptionLength();
    size_t getValueStart();
    size_t getValueEnd();
    size_t getValueLength();
  }

#endif
