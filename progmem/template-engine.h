#pragma once
#ifndef TEMPLATE_ENGINE_H
  #define TEMPLATE_ENGINE_H

#include "Arduino.h"
#include "Regexp.h"
#include "debug.h"

struct G_SUBSTR_T {
  uint16_t substr_start;
  uint16_t substr_length;
  uint16_t substr_end;
};

struct G_MATCH_T {
  bool hasAttribute;
  bool hasCondition;
  uint16_t match_start;
  uint16_t match_length;
  uint16_t match_end;
  G_SUBSTR_T variable_name;
  G_SUBSTR_T attribute_name;
  G_SUBSTR_T attribute_value;
};

class template_engine {
  private:
    char sep_char = '@';
    
  public:
    template_engine();
    ~template_engine();
    void begin();
    void setFieldSeperator( const char c );
    bool process_line( char* line, G_MATCH_T &match );
    char hasTemplate( MatchState &ms, char* searchStr );
    void parceTemplateEntry( MatchState &ms, G_MATCH_T &match );
};

#endif