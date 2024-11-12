#pragma once
#ifndef MQTT_CLIENT
  #define MQTT_CLIENT

  #include <Arduino.h>
  #include <WiFiClient.h>
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  #include "template-engine.h"
  #include "mqtt_message-templates.h"
  #include "config.h"
  #include "secrets.h"


  static String g_unique_host_name( String _hostname ) {
    String mac_address = WiFi.macAddress().substring( 12 );
    mac_address.replace( ":", "" );
    return _hostname + "_" + mac_address;
  }


  static const uint16_t G_MAX_MQTT_MSG_LENGTH = 1024;
  static WiFiClient espClient;
  static PubSubClient client( espClient );

  class MQTTClient {
    public:
      G_MATCH_T match;

    private:
      char m_host_name[ G_CONF_HOSTNAME_LENGTH ];
      char m_unique_name[ G_CONF_UNIQUE_HOSTNAME_LENGTH ];
    public:
      MQTTClient();
      ~MQTTClient();
      void begin();
      void callLoop();
      bool publish_discovery_msg();
    private:
      void SAFE_PRINT( char* _line );
      bool process_discovery_template( char *_line, uint16_t result_size );
      bool replace_str( char* _line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size );
  };

  extern MQTTClient g_mqtt;
#endif