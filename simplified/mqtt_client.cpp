#include <memory>
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
  m_provisioning = true;

  strlcpy( m_host_name, G_CONF_HOSTNAME, G_HOSTNAME_LENGTH );
  strlcpy( m_unique_name, g_unique_host_name( String( G_CONF_HOSTNAME ) ).c_str(), G_HOSTNAME_LENGTH + 6 );

  client->setCallback( callback );

  client->setServer( G_MQTT_SERVER, G_MQTT_SERVER_PORT );
  client->setBufferSize( G_MAX_MQTT_MSG_LENGTH );

  g_callback_msg.processed = true;
}


void MQTTClient::call_loop() {
  if ( ! client->connected() )
    connect();
  if ( espClient->connected() && ( WiFi.status() == WL_CONNECTED ) )
    client->loop();

  if ( g_callback_msg.processed == false && m_provisioning == false )
    decodeMessage();
}


bool MQTTClient::connect() {
  reconnect();
  if ( ! client->connected() ) return false;

  publish_stats();
  // if ( ! publish_stats() ) return false;

  if ( ! publish_discovery_msg() ) return false;

  subscribe();

  return true;
}


void MQTTClient::disconnect() {
  if ( G_SUBSCRIBED )
    unsubscribe();
  if ( client->connected() )
    client->disconnect();
}


bool MQTTClient::subscribe() {

  if ( !client->connected() )
    return false;

  if ( G_SUBSCRIBED )
    return true;

  char* _topic = new char[ strlen( G_MQTT_CMND_TOPIC ) + strlen( m_unique_name ) ];

  strlcpy( _topic, G_MQTT_CMND_TOPIC, G_MAX_MQTT_MSG_LENGTH );
  if ( process_template( _topic, G_MAX_MQTT_MSG_LENGTH ) )
    G_SUBSCRIBED = client->subscribe( _topic );

  delete _topic;

  return G_SUBSCRIBED;
}


void MQTTClient::unsubscribe() {

  if ( !client->connected() || !G_SUBSCRIBED )
    return;

  char* _topic = new char[ strlen( G_MQTT_CMND_TOPIC ) + strlen( m_unique_name ) ];

  if ( process_template( _topic, G_MAX_MQTT_MSG_LENGTH ) )
    if ( client->unsubscribe( _topic ) )
      G_SUBSCRIBED = false;

  delete _topic;
}


bool MQTTClient::publish_stats( const char* topic ) {
  if ( !client->connected() )
    return false;

  char* _topic = new char[ G_MAX_MQTT_TOPIC_LENGTH ];
  char* _msg = new char[ G_MAX_MQTT_MSG_LENGTH ];
  m_provisioning = true;
  bool _retVal = false;

  for ( uint8_t i = 0; i < sizeof( G_STAT_MESSAGES ) / sizeof( STAT_MESSAGE_T ); i++ ) {
    strlcpy( _topic, G_STAT_MESSAGES[ i ].TOPIC, G_MAX_MQTT_TOPIC_LENGTH );
    _msg[0] = '\0';

    // check if we are asked to process just a single message.
    // if so, if we are not at that message, then continue to the next iteration
    if ( topic != nullptr )
      if ( strcmp(_topic, topic ) != 0 )
        continue;

    _retVal = process_template( _topic, G_MAX_MQTT_TOPIC_LENGTH );

    if ( !_retVal )
      break;

    switch ( G_STAT_MESSAGES[ i ].CONFIG_KEY ) {
      case G_CONF_TIMEZONE:
        // g_conf.getAttrAsString( G_CONF_TIMEZONE, _msg, G_MAX_MQTT_MSG_LENGTH, "OPTION", g_conf.getTimeZone() );
        strlcpy( _msg, String("Dallas").c_str(), G_MAX_MQTT_MSG_LENGTH );
        break;
      case G_CONF_LTR_MIN:
      case G_CONF_LTR_MAX:
      case G_CONF_LED_BRIGHTNESS_MIN:
      case G_CONF_LED_BRIGHTNESS_MAX:
        // g_conf.getValueAsString( G_STAT_MESSAGES[ i ].CONFIG_KEY, _msg, G_MAX_MQTT_MSG_LENGTH );
        snprintf( _msg, G_MAX_MQTT_MSG_LENGTH, "%u", random(100) );
        break;
      case G_CONF_FIRMWARE_SEEED: // pseudonym used to branch out to send led status message
        strlcpy( _msg, G_LED_STATUS, G_MAX_MQTT_MSG_LENGTH );
        break;
      case G_CONF_RING_1_BACKGROUND_COLOR:
      case G_CONF_RING_2_BACKGROUND_COLOR:
      case G_CONF_RING_3_BACKGROUND_COLOR:
      case G_CONF_RING_4_BACKGROUND_COLOR:
        // _retVal = g_conf.getColorAsRGB( G_STAT_MESSAGES[ i ].CONFIG_KEY, _msg, G_MAX_MQTT_MSG_LENGTH );
        strlcpy( _msg, String("111,222,333").c_str(), G_MAX_MQTT_MSG_LENGTH );
        break;
    }

    if ( strlen( _msg ) == 0 || strlen( _topic ) == 0 )
      continue;

    // if ( _retVal )
    //   _retVal = client->publish( _topic, _msg, true );
  }

  m_provisioning = false;

  delete _msg;
  delete _topic;

  return _retVal;
}


bool MQTTClient::publish_discovery_msg() {
  if ( !client->connected() )
    return false;

  bool _retVal = false;
  char* _topic = new char[ G_MAX_MQTT_TOPIC_LENGTH ];
  char* _msg = new char[ G_MAX_MQTT_MSG_LENGTH ];
  uint8_t _repCount = sizeof( G_DISCOVERY_MESSAGES ) / sizeof( DISCOVERY_MESSAGE_T );

  m_provisioning = true;
  for ( uint8_t i = 0; i < _repCount; i++ ) {
    strlcpy( _topic, G_DISCOVERY_MESSAGES[ i ].TOPIC, G_MAX_MQTT_TOPIC_LENGTH );

    if ( !process_template( _topic, G_MAX_MQTT_TOPIC_LENGTH ) )
      break;

    strlcpy( _msg, G_DISCOVERY_MESSAGES[ i ].MESSAGE, G_MAX_MQTT_MSG_LENGTH);
    if ( !process_template( _msg, G_MAX_MQTT_MSG_LENGTH ) )
      break;

    // _retVal = client->publish( _topic, _msg, true );
  }
  m_provisioning = false;

  delete _msg;
  delete _topic;

  return _retVal;
}


bool MQTTClient::publish_lux_levels( uint16_t lux_level, uint16_t average_lux_level, const char* date_time ) {
  if ( !client->connected() || m_provisioning )
    return false;

  bool _retVal = false;

  char _mqtt_topic[ strlen( m_unique_name ) + 1 ];
  char _msg[ 120 ];
  uint8_t _val_count = 3;
  arg_vals_t vals[ _val_count ];

  strlcpy( _mqtt_topic, G_MQTT_TELE_TOPIC, G_MAX_MQTT_MSG_LENGTH );
  if ( !process_template( _mqtt_topic, G_MAX_MQTT_MSG_LENGTH ) )
    return false;

  strlcpy( _msg, G_MQTT_TELE, G_MAX_MQTT_MSG_LENGTH );

  char _key0[ strlen( "date_time" ) + 1 ] = "date_time";
  vals[ 0 ].arg_value_size = strlen( date_time ) + 1;
  vals[ 0 ].arg_name = &_key0[ 0 ];
  vals[ 0 ].arg_value = date_time;

  char _key1[ strlen( "current_lux_level" ) + 1 ] = "current_lux_level";
  char _val1[ 6 ];
  itoa( lux_level, _val1, 10 );
  vals[ 1 ].arg_value_size = 6;
  vals[ 1 ].arg_name = &_key1[ 0 ];
  vals[ 1 ].arg_value = &_val1[ 0 ];

  char _key2[ strlen( "average_lux_level" ) + 1 ] = "average_lux_level";
  char _val2[ 6 ];
  itoa( average_lux_level, _val2, 10 );
  vals[ 2 ].arg_value_size = 6;
  vals[ 2 ].arg_name = &_key2[ 0 ];
  vals[ 2 ].arg_value = &_val2[ 0 ];

  if ( ! process_template( _msg, G_MAX_MQTT_MSG_LENGTH, vals, _val_count ) )
    return false;

  // _retVal = client->publish( _mqtt_topic, _msg );

  return _retVal;
}


bool MQTTClient::setBrightness( uint8_t brightness ) {
  if ( !client->connected() || m_provisioning )
    return false;

  bool _retVal = false;

  char* _topic = new char[ G_MAX_MQTT_TOPIC_LENGTH ];
  char _msg[ 4 ];

  strlcpy( _topic, G_MQTT_STAT_LED_BRIGHTNESS, G_MAX_MQTT_TOPIC_LENGTH );
  if ( process_template( _topic, G_MAX_MQTT_TOPIC_LENGTH ) ) {
    itoa( brightness, _msg, 10 );
    // _retVal = client->publish( _topic, _msg );
  }

  delete _topic;

  return _retVal;
}


/* ***************************************************************** Private support functions ***************************************************************** */

/* a poor mans MAP function.
  There's unfortunately not a currently maintain implementation of C++ Standard template library
  This is a brute force implementation of mapping from String to enum
*/
TMPL_STRINGS_E MQTTClient::getTmplFromString( char* key ) {

  for ( TMPL_STRINGS_E tmpl = TMPL_STRINGS_E( KEY_ITERATER_FIRST + 1 ); tmpl < KEY_ITERATE_LAST; TMPL_STRINGS_E( tmpl + 1 ) ) {
    if ( strcmp( key, G_TEMPLATE_KEYWORDS[ tmpl ] ) == 0 ) {
      return tmpl;
      break;
    }
  }
  return KEY_UNKNOWN;
}


void MQTTClient::reconnect() {
  if ( !client->connected() && G_ATTEMPTS < G_MAX_CONNECT_ATTEMPTS ) {
    Serial.print("Attempting MQTT connection ...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client->connect("ESP8266Client")) {
    if ( client->connect( "ESP8266Client", G_CONF_MQTT_USER, G_CONF_MQTT_PASS ) )  {
      Serial.printf( "Connected to MQTT Server\n", nullptr );
    } else {
      G_ATTEMPTS += 1;
      Serial.print( " failed, rc=" );
      Serial.println( client->state() );
    }
  }
}


void MQTTClient::decodeMessage() {
  if ( g_callback_msg.processed == true )
    return;
  g_callback_msg.processed = true;    // make sure we don't get called repeatedly. So change the flag, eventhough we are still processing

  DynamicJsonDocument _doc(254);

  Serial.printf( "%s\n", g_callback_msg.payload );
  DeserializationError error = deserializeJson( _doc, g_callback_msg.payload );

  if ( error ) {
    Serial.printf( "deserializeJson() failed: '%s'\n", error.f_str() );
    return;
  }

  if ( _doc.containsKey( "tz" ) ) {
    // g_conf.setValue( G_CONF_TIMEZONE, (uint8_t)_doc[ "tz" ] );
    publish_stats( G_MQTT_STAT_TZ_TOPIC );
  }

  g_callback_msg.payload_length = 0;
}


bool MQTTClient::processCommand( String cmnd, String param ) {
  bool _retVal = false;
  Serial.println( cmnd );
  Serial.println( param );
  return _retVal;
}


bool MQTTClient::process_template( char* line, uint16_t line_size, arg_vals_t* vals, uint8_t val_count ) {

  char* _replaceVal = new char[ line_size ];
  TMPL_STRINGS_E templ_key;
  bool _retVal = false;

  TMPL_ENGINE::reset();

  while ( TMPL_ENGINE::hasTemplate( line ) ) {

    if ( TMPL_ENGINE::getVariableEnd() > strlen( line ) )
      break;

    strlcpy( _replaceVal, line + TMPL_ENGINE::getVariableStart(), TMPL_ENGINE::getVariableLength() + 1 ); // _length +1 to include space for string termination character '\0'
    templ_key = getTmplFromString( _replaceVal );

    if ( templ_key == KEY_UNKNOWN )
      break;

    switch ( templ_key ) {
      case KEY_UNIQUE_NAME:
        _retVal = TMPL_ENGINE::replace_str( line, m_unique_name,  TMPL_ENGINE::getMatchStart(),  TMPL_ENGINE::getMatchEnd(), line_size );
        break;

      case KEY_DATE_TIME:
      case KEY_CURRENT_LUX_LEVEL:
      case KEY_AVERAGE_LUX_LEVEL:
        if ( vals != nullptr )
          for ( uint8_t i = 0; i < val_count; i++ ) {
            if ( strcmp( _replaceVal, vals[ i ].arg_name ) == 0 ) {
              _retVal = TMPL_ENGINE::replace_str( line, vals[ i ].arg_value, TMPL_ENGINE::getMatchStart(), TMPL_ENGINE::getMatchEnd(), line_size );
              break;
            }
          }
        break;

      case KEY_HARDWARE_VERSION:
        _retVal = TMPL_ENGINE::replace_str( line, G_HARDWARE_VERSION, TMPL_ENGINE::getMatchStart(),  TMPL_ENGINE::getMatchEnd(), line_size );
        break;

      case KEY_FIRMWARE_VERSION:
        _retVal = TMPL_ENGINE::replace_str( line, G_FIRMWARE_VERSION, TMPL_ENGINE::getMatchStart(),  TMPL_ENGINE::getMatchEnd(), line_size );
        break;

      case KEY_IP_ADDRESS:
        char _ipAddr[ 23 ];

        sprintf( _ipAddr, "http://%s", espClient->localIP().toString().c_str() );
        _retVal = TMPL_ENGINE::replace_str( line, _ipAddr, TMPL_ENGINE::getMatchStart(),  TMPL_ENGINE::getMatchEnd(), line_size );
        break;

      case KEY_TIMEZONE:
        uint16_t str_size = ( G_TIMEZONE_NAME_LENGTH + 3 ) * G_TIMEZONE_OPTIONS_COUNT + 1;
        char _tmp[ str_size ] = "\0";
        uint16_t _pos = 0;
        for ( uint8_t i = 0; i < G_TIMEZONE_OPTIONS_COUNT; i++ ) {
          snprintf( &_tmp[ _pos ], str_size - _pos, "\"%s %u\",", String("timezone").c_str(), i );
          _pos = strlen( _tmp ) + 1;
        }
        _tmp[  _pos - 2 ] = '\0';  // remove trailing ","

        _retVal = TMPL_ENGINE::replace_str( line, _tmp, TMPL_ENGINE::getMatchStart(),  TMPL_ENGINE::getMatchEnd(), line_size );
        break;
    }

    if (!_retVal ) // something went wrong processing the line, so escape the while loop
      break;

  }

  delete _replaceVal;

  return _retVal;
}
