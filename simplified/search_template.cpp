#include "search_template.h"


namespace SEARCH_TMPL {

  char _delim[3];
  size_t match_start, match_end, match_variable_start, match_variable_end;


  void begin() {
    set_search_chr( '@' );
  }


  void reset() {
    match_start = 0xffff;
    match_end = 0xffff;
    match_variable_start = 0xffff;
    match_variable_end = 0xffff;
  }


  void set_search_chr( char c ) {
    sprintf( _delim, "%c%c", c, c );
  }


  bool hasTemplate( char* line ) {
    bool _retVal = false;
    size_t _start, _end;
    
    if ( find_key( line, _start ) )                   // look for start of template string
      if ( find_key( &line[ _start + 2 ], _end ) ) {  // look for matching end of template string
        match_start = _start;
        match_variable_start = _start + 2;
        match_variable_end = match_variable_start + _end;
        match_end = match_variable_end + 2;
        _retVal = true;
    }
      
    return _retVal;
  }


  bool find_key( char* line, size_t &pos ) {
    char* _needle;

    _needle = strstr( line, _delim );
    if ( !_needle )
      return false;
    
    pos = _needle - line;
    return true;
  }


  bool replace_str( char* line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size ) {
    size_t _line_length = strlen( line );
    size_t _new_val_length = strlen( new_val );

    if ( !checkBounds( line_size, _line_length, (end_pos - start_pos ), _new_val_length ) ) {
      return false;
    }

    if ( ( end_pos - start_pos ) != _new_val_length ) {
      memmove( (void*)(line + start_pos + _new_val_length), (void*)(line + end_pos), _line_length - end_pos + 1);
    }
    
    memcpy( (void*)(line + start_pos), (void*)new_val, _new_val_length );
    return true;
  }


  bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t replace_length ) {
    return haystack_size > ( haystack_length - needle_length + replace_length );
  }

  
  size_t getMatchStart() { return match_start; }
  size_t getMatchEnd() { return match_end; }
  size_t getMatchLength() { return match_end - match_start; }
  size_t getVariableStart() { return match_variable_start; }
  size_t getVariableEnd() { return match_variable_end; }
  size_t getVariableLength() { return match_variable_end - match_variable_start; }
}