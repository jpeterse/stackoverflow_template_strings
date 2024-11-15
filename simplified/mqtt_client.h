#pragma once
#ifndef MQTT_CLIENT_H
  #define MQTT_CLIENT_H

  #include <Arduino.h>
  #include <WiFiClient.h>
  #include <PubSubClient.h>
  #include "tmpl_strings.h"
  #include "search_template.h"
  #include "secrets.h"

  static bool g_subscribed;
  static const uint8_t G_MAX_CONNECT_ATTEMPTS = 5;
  static uint8_t g_attempts = 0;

  class MQTTClient {
    public:
      static const uint16_t G_MAX_MQTT_MSG_LENGTH = 1024;
    private:
      WiFiClient* espClient;
      PubSubClient* client;
    public:
      MQTTClient();
      ~MQTTClient();
      void begin();
      void call_loop();
      void reconnect();
      bool connect();
      void disconnect();
      bool subscribe();
      void unsubscribe();
      void publish_discovery_messages();
    private:
      bool process_line( char* line, uint16_t line_size );
      bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t replace_length );
      bool replace_str( char* _line, const char* new_val, uint16_t start_pos, uint16_t end_pos, uint16_t line_size );
  };

#endif