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

  size_t getMatchStart() { return match_start; }
  size_t getMatchEnd() { return match_end; }
  size_t getMatchLength() { return match_end - match_start; }
  size_t getVariableStart() { return match_variable_start; }
  size_t getVariableEnd() { return match_variable_end; }
  size_t getVariableLength() { return match_variable_end - match_variable_start; }
}