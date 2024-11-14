#include "tmpl_strings.h"

void setup() {
  Serial.begin( 19200 );
  Serial.setDebugOutput( false );
  while ( !Serial && millis() < 5000 )
    yield();
  Serial.println();

  read_strs();

  Serial.println( "Done" );
}

void loop() { yield(); }

// for each title and message in tmplt_strs, replace the template values and print
void read_strs() {
  const uint16_t _line_size = 1024;
  char _title[ _line_size ];
  char _msg[ _line_size ];
  uint8_t _repCount = sizeof( tmplt_strs ) / sizeof( tmplt_str_t );
  
  for ( uint8_t i = 0; i < _repCount; i++ ) {
    strlcpy( _title, tmplt_strs[ i ].title, _line_size );
    // Serial.printf( "[%s]\t%d:\t%p\t%p\t%s\n", __FUNCTION__, __LINE__, _title, _title[ 0 ], _title );
    if ( !process_line( _title, _line_size ) )
      break;

    strlcpy( _msg, tmplt_strs[ i ].msg, _line_size);
    if ( !process_line( _msg, _line_size ) )
      break;

    Serial.printf( "[%s]\t%d:\t%s\n", __FUNCTION__, __LINE__, _title );
    Serial.printf( "[%s]\t%d:\t%s\n", __FUNCTION__, __LINE__, _msg );

  }
}


bool process_line( char line[], uint16_t line_size ) {
  bool _retVal = false;
  uint16_t _start, _length;
  char _replaceVal[ line_size ];
  
  if ( !find_key( line, _start ) )
    return false;
  if ( _start >= strlen( line ) )
    return false;
  if ( !find_key( &line[ _start + 2 ], _length ) )
    return false;
  
  _length += 4; // adjust for starting and ending '@' characters.
  // Serial.printf( "[%s]\t%d:\t%p\t%p\t%s\n", __FUNCTION__, __LINE__, line, line[ 0 ], line );
  // Serial.printf( "[%s]\t%d:\tmatch start: %d\tmatch length: %d\n", __FUNCTION__, __LINE__, _start, _length );

  strlcpy( _replaceVal, &line[ 0 ] + _start + 2, _length - 4 + 1 );

  if ( strcmp( _replaceVal, String("first").c_str() ) == 0 )
    _retVal = replace_str( line, String("1.").c_str(), _start, _start + _length, line_size );
  if ( strcmp( _replaceVal, String("second").c_str() ) == 0 )
    _retVal = replace_str( line, String("2.").c_str(), _start, _start + _length, line_size );
  if ( strcmp( _replaceVal, String("third").c_str() ) == 0 )
    _retVal = replace_str( line, String("3.").c_str(), _start, _start + _length, line_size );
  if ( strcmp( _replaceVal, String("replace").c_str() ) == 0 )
    _retVal = replace_str( line, String("sit amet,").c_str(), _start, _start + _length, line_size );
  if ( strcmp( _replaceVal, String("me").c_str() ) == 0 )
    _retVal = replace_str( line, String("lacinia tortor, id vehicula").c_str(), _start, _start + _length, line_size );
  if ( strcmp( _replaceVal, String("and me").c_str() ) == 0 )
    _retVal = replace_str( line, String("venenatis").c_str(), _start, _start + _length, line_size );

  return _retVal;
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


bool find_key( char* line, uint16_t &pos ) {
char* _needle;

  _needle = strstr( line, "@@" );
  if ( !_needle )
    return false;
  
  pos = _needle - line;
  return true;
}


bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t replace_length ) {
  return haystack_size > ( haystack_length - needle_length + replace_length );
}