#include <WiFiManager.h>
#include "template-engine.h"
#include "mqtt-client.h"
#include "debug.h"

const bool G_FACTORY_RESET = false;
bool G_MQTT_ENABLE = true;

WiFiManager wifimanager;
std::unique_ptr<MQTTClient> g_mqtt_client{};


void setup() {

  Serial.begin( 19200 );
  Serial.setDebugOutput( false );
  while ( !Serial && millis() < 5000 )
    yield();
  Serial.println();

  connectToWiFi();

  if ( G_MQTT_ENABLE && WiFi.isConnected() ) {
    g_mqtt_client = std::make_unique<MQTTClient>();
    g_mqtt_client->begin();
    g_mqtt_client->publish_discovery_msg();
    // g_mqtt_client->connect();
  }
}


void loop() {
  g_mqtt_client->callLoop();
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
      Serial.println( "Unable to connect to WiFi network using saved network information");
    else
      Serial.println( "Unable to connect to WiFi");
    return false;
  }

  DEBUG_MSG( "Connected with IP: %s\n", WiFi.localIP().toString() );

  return true;
}