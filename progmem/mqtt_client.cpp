#include "mqtt-client.h"

MQTTClient::MQTTClient() {}


MQTTClient::~MQTTClient() {}


void MQTTClient::begin() {
  // G_* value are stored define in secrets.h
  client.setServer( G_CONF_MQTT_SERVER, G_CONF_MQTT_SERVER_PORT );
  client.setBufferSize( G_MAX_MQTT_MSG_LENGTH );
  strlcpy( m_host_name, G_CONF_HOSTNAME, G_CONF_HOSTNAME_LENGTH );
  strlcpy( m_unique_name, g_unique_host_name( String( G_CONF_HOSTNAME ) ).c_str(), G_CONF_UNIQUE_HOSTNAME_LENGTH );
  DEBUG_MSG("MQTTClient initialized\n", nullptr );
}

/* FOR SIMPLICITY, CONNECT, RECONNECT, DISCONNECT, SUBSCRIBE, UNSUBSCRIBE, and CALLBACK functions has been removed */

void MQTTClient::callLoop() {
  // if ( !client.connected() )
  //   connect();
  // if ( espClient.connected() && ( WiFi.status() == WL_CONNECTED ) )
  //   client.loop();
}


bool MQTTClient::publish_discovery_msg() {
  // if ( ! subscribe() ) return false;

  bool _retVal = false;
  uint16_t _line_size = G_MAX_MQTT_MSG_LENGTH;
  char _topic[ _line_size ];
  char* _topic_ptr = &_topic[0];
  char _msg[ _line_size ];
  char* _msg_ptr = &_msg[0];
  // const char* _progmem_str;
  uint8_t _i;

  // G_DISCOVERY_MESSAGES* are defined in mqtt_message-templates.h
  do {
    // _progmem_str = (const char*)pgm_read_ptr( &( G_DISCOVERY_MESSAGES[ _i ].TOPIC ) );
    // strcpy_P( _topic, _progmem_str );
    // _topic[ strlen_P( _progmem_str ) + 1 ] = '\0';
    strlcpy( _topic,  G_DISCOVERY_MESSAGES[ _i ].TOPIC, _line_size );

    // Process discovery template (if needed)    
    _retVal = process_discovery_template( _topic_ptr, _line_size );
    if ( _retVal ) {
      // _progmem_str = (const char*)pgm_read_ptr( &( G_DISCOVERY_MESSAGES[ _i ].MESSAGE ) );
      // strcpy_P( _msg, _progmem_str );
      strlcpy( _msg, G_DISCOVERY_MESSAGES[ _i ].MESSAGE, _line_size );
      _retVal = process_discovery_template( _msg_ptr, _line_size );
    }
    // if ( _retVal )
    //   _retVal = client.publish( _topic, _msg, true );    // disabled for debugging, as we don't connect to server
    Serial.println( _topic );
    Serial.println( _msg );
    Serial.print( "Free Heap: ");
    Serial.print( ESP.getFreeHeap() );
    Serial.print( " Heap Fragmentation: " );
    Serial.println( ESP.getHeapFragmentation() );

  } while ( ++_i < G_DISCOVERY_MESSAGE_COUNT && _retVal );

  return _retVal;
}


/* ****** PRIVATE SUPPORT FUNCTIONS ****** */


bool MQTTClient::process_discovery_template( char* line, uint16_t result_size ) {
  // if ( ! subscribe() ) return false;

  G_MATCH_T match;
  bool _hasMatch = false;
  bool _retVal = false;
  char _variable[ result_size ];
  bool match_found;
  template_engine _tmpl_engine;
  
  Serial.println( ESP.getFreeHeap() );
  _tmpl_engine.begin();
  
  _hasMatch = _tmpl_engine.process_line( line, match );
  if ( !_hasMatch || match.match_length == 0 )
    return true;

  do {
    strlcpy( _variable, line + match.variable_name.substr_start, match.variable_name.substr_length + 1 );  // +1 to make room for the trailing '\0'
    if ( strcmp( _variable, "unique_name" ) == 0 ) {
      replace_str( line, m_unique_name, match.match_start, match.match_end, result_size );
      _retVal = true;
    }

    if ( strcmp( _variable, "hardware_version" ) == 0 ) {
      replace_str( line, G_HARDWARE_VERSION, match.match_start, match.match_end, result_size );
      _retVal = true;
    }

    if ( strcmp( _variable, "firmware_version" ) == 0 ) {
      replace_str( line, G_FIRMWARE_VERSION, match.match_start, match.match_end, result_size );
      _retVal = true;
    }

    if ( strcmp( _variable, "ip_address" ) == 0 ) {
      char _ipAddr[ 23 ];
      sprintf( _ipAddr, "http://%s", WiFi.localIP().toString().c_str() );
      _retVal = replace_str(  line, _ipAddr, match.match_start, match.match_end, result_size );
    }

    if ( strcmp(_variable, "timezones" ) == 0 ) {
      uint8_t _size = 15;
      char _placeholder[ _size ];
      strlcpy( _placeholder, "placeholder", _size );
      _retVal = replace_str( line, _placeholder, match.match_start, match.match_end, result_size );
    }

    if ( _retVal ) {
      _hasMatch = _tmpl_engine.process_line( line, match );
    }

  } while ( _retVal && _hasMatch && match.match_length > 0 );

  return _retVal;
}


bool MQTTClient::replace_str( char* line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size ) {
  size_t line_length = strlen( line );
  size_t new_val_length = strlen( new_val );

  if ( line_size <= (line_length - (end_pos - start_pos ) + new_val_length ) ) {
    return false;
  }

  if ( ( end_pos - start_pos ) != new_val_length ) {
    memmove( (void*)(line + start_pos + new_val_length), (void*)(line + end_pos), line_length - end_pos + 1);
  }
  
  memcpy( (void*)(line + start_pos), (void*)new_val, new_val_length );

  return true;
}
