#include "mqtt_client.h"


MQTTClient::MQTTClient() {
  espClient = new WiFiClient();
  client = new PubSubClient( *espClient );
}


MQTTClient::~MQTTClient() {
  delete client;
  delete espClient;
  client = NULL;
  espClient = NULL;
}


void MQTTClient::begin() {
  client->setServer( G_CONF_MQTT_SERVER, G_CONF_MQTT_SERVER_PORT );
  client->setBufferSize( G_MAX_MQTT_MSG_LENGTH );
}


void MQTTClient::call_loop() {
  yield();
}


void MQTTClient::reconnect() {
  if ( !client->connected() && g_attempts < G_MAX_CONNECT_ATTEMPTS ) {
    Serial.print("Attempting MQTT connection ...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if ( client->connect( "ESP8266Client", G_CONF_MQTT_USER, G_CONF_MQTT_PASS ) ) {
      Serial.println( "Connected to MQTT Server" );
    } else {
      g_attempts += 1;
      Serial.print( " failed, rc=" );
      Serial.println( client->state() );
    }
  }
}


bool MQTTClient::connect() {
  bool _retVal = false;
  
  reconnect();
  if ( client->connected() ) {
    // _retVal = publish_tz_state();
    // if ( _retVal ) {
      publish_discovery_messages();
    // }
  }

  return _retVal;
}


void MQTTClient::disconnect() {
  if ( g_subscribed )
    unsubscribe();
  if ( client->connected() )
    client->disconnect();
}


bool MQTTClient::subscribe() {
  // char _mqtt_topic[ strlen( m_mqtt_topic_t ) + strlen( m_unique_name ) + 5 ];

  // sprintf( _mqtt_topic, m_mqtt_topic_t, String( "cmnd" ).c_str(), m_unique_name );

  // if ( !client.connected() ) {
  //   g_subscribed = false;
  //   connect();
  // }
  // if ( client.connected() ) // don't assume reconnection was successfull if attempted above, verify it
  //   if ( !g_subscribed ) {
  //     Serial.printf( "Subscribing to: %s\n", _mqtt_topic );
  //     g_subscribed = client.subscribe( _mqtt_topic );
  //   }
  return g_subscribed;
}


void MQTTClient::unsubscribe() {
  // char _mqtt_topic[ strlen( m_mqtt_topic_t ) + strlen( m_unique_name ) + 5 ];

  // sprintf( _mqtt_topic, m_mqtt_topic_t, String( "cmnd" ).c_str(), m_unique_name );

  // if ( !client.connected() ) {
  //   g_subscribed = false;
  //   connect();
  // }
  // if ( client.connected() ) // don't assume reconnection was successfull if attempted above, verify it
  //   if ( g_subscribed )
  //     if ( client.unsubscribe( _mqtt_topic ) )
  //       g_subscribed = false;
}


void MQTTClient::publish_discovery_messages() {

  if ( !client->connected() )
    return;

  const uint16_t _line_size = G_MAX_MQTT_MSG_LENGTH;
  char _topic[ _line_size ];
  char _msg[ _line_size ];
  uint8_t _repCount = sizeof( tmplt_strs ) / sizeof( tmplt_str_t );
  
  for ( uint8_t i = 0; i < _repCount; i++ ) {
    strlcpy( _topic, tmplt_strs[ i ].title, _line_size );
    // Serial.printf( "[%s]\t%d:\t%p\t%p\t%s\n", __FUNCTION__, __LINE__, _title, _title[ 0 ], _title );
    if ( !process_line( _topic, _line_size ) )
      break;

    strlcpy( _msg, tmplt_strs[ i ].msg, _line_size);
    if ( !process_line( _msg, _line_size ) )
      break;

    Serial.printf( "[%s]\t%d:\t%s\n", __FUNCTION__, __LINE__, _topic );
    Serial.printf( "[%s]\t%d:\t%s\n", __FUNCTION__, __LINE__, _msg );
    client->publish( _topic, _msg );
  }
}


/* ********* Private support functions ********* */

bool MQTTClient::process_line( char* line, uint16_t line_size ) {
  bool _retVal = false;
  size_t _start, _length;
  char _replaceVal[ line_size ];

  SEARCH_TMPL::begin();

  while ( SEARCH_TMPL::hasTemplate( line ) ) {

    strlcpy( _replaceVal, line + SEARCH_TMPL::getVariableStart(), SEARCH_TMPL::getVariableLength() + 1 ); // _length +1 to include space for string termination character '\0'

    if ( strcmp( _replaceVal, String("unique_name").c_str() ) == 0 )
      _retVal = replace_str( line, G_CONF_UNIQUE_NAME,  SEARCH_TMPL::getMatchStart(),  SEARCH_TMPL::getMatchEnd(), line_size );
    if ( strcmp( _replaceVal, String("hardware_version").c_str() ) == 0 )
      _retVal = replace_str( line, G_HARDWARE_VERSION, SEARCH_TMPL::getMatchStart(),  SEARCH_TMPL::getMatchEnd(), line_size );
    if ( strcmp( _replaceVal, String("firmware_version").c_str() ) == 0 )
      _retVal = replace_str( line, G_FIRMWARE_VERSION, SEARCH_TMPL::getMatchStart(),  SEARCH_TMPL::getMatchEnd(), line_size );
    if ( strcmp( _replaceVal, String("ip_address").c_str() ) == 0 ){
      char _ipAddr[ 23 ];
      
      sprintf( _ipAddr, "http://%s", espClient->localIP().toString().c_str() );
      _retVal = replace_str( line, _ipAddr, SEARCH_TMPL::getMatchStart(),  SEARCH_TMPL::getMatchEnd(), line_size );
    }
    if ( strcmp( _replaceVal, String("timezones").c_str() ) == 0 ){
      uint8_t _size = 15;
      char _placeholder[ _size ];
      strlcpy( _placeholder, "placeholder", _size );
      _retVal = replace_str( line, _placeholder, SEARCH_TMPL::getMatchStart(),  SEARCH_TMPL::getMatchEnd(), line_size );
    }

    _start = 0;
  }

  return _retVal;
}


bool MQTTClient::checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t replace_length ) {
  return haystack_size > ( haystack_length - needle_length + replace_length );
}


bool MQTTClient::replace_str( char* line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size ) {
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