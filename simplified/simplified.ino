#include <WiFiManager.h>
#include "secrets.h"
#include "tmpl_strings.h"

WiFiManager wifimanager;

bool G_FACTORY_RESET = false;

void setup() {
  Serial.begin( 19200 );
  Serial.setDebugOutput( false );
  while ( !Serial && millis() < 5000 )
    yield();
  Serial.println();

  connectToWiFi();

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


bool process_line( char* line, uint16_t line_size ) {
  bool _retVal = false;
  uint16_t _start, _length;
  char _replaceVal[ line_size ];
  
  while ( find_key( line, _start ) ) {
    
    if ( _start >= strlen( line ) )
      break;
    if ( !find_key( &line[ _start + 2 ], _length ) )  // +2 to start new search after the already matched "@@"
      break;

    strlcpy( _replaceVal, line + _start + 2, _length + 1 ); // _length +1 to include space for string termination character '\0'

    _length += 4; // adjust length to include leading and trailing '@' characters.

    if ( strcmp( _replaceVal, String("unique_name").c_str() ) == 0 )
      _retVal = replace_str( line, G_CONF_UNIQUE_NAME, _start, _start + _length, line_size );
    if ( strcmp( _replaceVal, String("hardware_version").c_str() ) == 0 )
      _retVal = replace_str( line, G_HARDWARE_VERSION, _start, _start + _length, line_size );
    if ( strcmp( _replaceVal, String("firmware_version").c_str() ) == 0 )
      _retVal = replace_str( line, G_FIRMWARE_VERSION, _start, _start + _length, line_size );
    if ( strcmp( _replaceVal, String("ip_address").c_str() ) == 0 ){
      char _ipAddr[ 23 ];
      sprintf( _ipAddr, "http://%s", WiFi.localIP().toString().c_str() );
      _retVal = replace_str(  line, _ipAddr, _start, _start + _length, line_size );
    }
    if ( strcmp( _replaceVal, String("timezones").c_str() ) == 0 ){
      uint8_t _size = 15;
      char _placeholder[ _size ];
      strlcpy( _placeholder, "placeholder", _size );
      _retVal = replace_str( line, _placeholder, _start, _start + _length, line_size );
    }

    _start = 0;
  }

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


/* connect to wifi network */
bool connectToWiFi() {
  bool _retVal;

  WiFi.setHostname( G_CONF_HOSTNAME );

  if ( G_FACTORY_RESET )
    wifimanager.resetSettings();                      // clear any stored wifi configuration

  WiFi.persistent( true );                            // reconnect to the previously connected access point.
  WiFi.setAutoReconnect( true );                      // make WiFi automatically reconnect if connection is lost
  wifimanager.setClass( "invert" );                   // set dark theme for WiFiManager configuration page

  _retVal = wifimanager.autoConnect( G_CONF_WIFI_AP_NAME, G_CONF_WIFI_AP_PASS );

  if ( !_retVal ) {
    if ( wifimanager.getWiFiIsSaved() )
      Serial.println( "Unable to connect to WiFi network using saved network information" );
    else
      Serial.println( "Unable to connect to WiFi" );
    return false;
  }

  Serial.printf( "Connected with IP: %s\n", WiFi.localIP().toString() );

  return true;
}