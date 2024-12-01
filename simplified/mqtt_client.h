#pragma once
#ifndef MQTT_CLIENT
  #define MQTT_CLIENT

  #include <Arduino.h>
  #include <WiFiClient.h>
  #include <PubSubClient.h>
  #include <ArduinoJson.h>
  #include "template_engine.h"
  #include "tmpl_strings.h"
  #include "config.h"
  #include "secrets.h"


  struct callback_message_t {
    bool processed;
    char payload[ G_MAX_MQTT_MSG_LENGTH + 1 ];
    int payload_length;
  };

  static const uint8_t G_MAX_CONNECT_ATTEMPTS = 5;
  static uint8_t G_ATTEMPTS = 0;
  static bool G_SUBSCRIBED = false;
  static callback_message_t g_callback_msg;


  static void callback( char* topic, byte* payload, unsigned int length ) {
    g_callback_msg.payload_length = length;
    for ( uint8_t i = 0; i < length; i++ )
      g_callback_msg.payload[ i ] = ( char )payload[ i ];
    g_callback_msg.payload[ length ] = '\0';
    g_callback_msg.processed = false;
  };


  class MQTTClient {

    public:
      struct arg_vals_t {
        const char* arg_name;
        const char* arg_value;
        size_t arg_value_size;
      };

    private:
      WiFiClient* espClient;
      PubSubClient* client;
      String macOctetToString( uint8_t octet );
      char m_host_name[ G_HOSTNAME_LENGTH ];
      char m_unique_name[ G_HOSTNAME_LENGTH + 6 ];
      bool m_provisioning = false;

    public:
      MQTTClient();
      ~MQTTClient();
      void begin();
      void call_loop();
      bool connect();
      void disconnect();
      bool subscribe();
      void unsubscribe();
      bool publish_discovery_msg();
      bool publish_stats( const char* topic = nullptr );
      bool publish_lux_levels( uint16_t lux_level, uint16_t average_lux_level, const char* date_time );
      bool setBrightness( uint8_t brightness );
    private:
      TMPL_STRINGS_E getTmplFromString( char* key );
      void reconnect();
      void decodeMessage();
      bool processCommand( String cmnd, String param );
      bool process_template( char* _line, uint16_t line_size, arg_vals_t* vals = nullptr, uint8_t val_count = 0 );
    };

#endif
