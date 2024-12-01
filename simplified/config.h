#pragma once
#ifndef CONFIG_H
  #define CONFIG_H
  
    #include <ESP8266WiFi.h>

    enum G_CONFIGDATA : uint8_t {
      G_CONF_FIRMWARE_SEEED,
      G_CONF_DEVICE_TYPE,
      G_CONF_HOST_NAME,
      G_CONF_LTR_INSTALLED,
      G_CONF_RING_1_BACKGROUND_COLOR,
      G_CONF_RING_2_BACKGROUND_COLOR,
      G_CONF_RING_3_BACKGROUND_COLOR,
      G_CONF_RING_4_BACKGROUND_COLOR,
      G_CONF_RING_1_SECTION_MARKS,
      G_CONF_SECOND_HAND_COLOR,
      G_CONF_MINUTE_HAND_COLOR,
      G_CONF_HOUR_HAND_COLOR,
      G_CONF_MONTHS_COLOR,
      G_CONF_DAYS_COLOR,
      G_CONF_DATE_BACKGROUND_COLOR,
      G_CONF_LTR_MIN,
      G_CONF_LTR_MAX,
      G_CONF_LTR_GAIN,
      G_CONF_LTR_INT,
      G_CONF_LED_BRIGHTNESS_MIN,
      G_CONF_LED_BRIGHTNESS_MAX,
      G_CONF_TIMEZONE,
      G_CONF_CLOCKMODE,
      G_CONF_MQTT_ENABLED,
      G_CONF_MQTT_SERVER,
      G_CONF_MQTT_SERVER_PORT,
      G_CONF_MQTT_USERNAME,
      G_CONF_MQTT_PASSWORD
    };

    const uint8_t G_HOSTNAME_LENGTH = 64;
    const uint8_t G_UNIQUE_HOSTNAME_LENGTH = 70;
    const static uint8_t G_TIMEZONE_OPTIONS_COUNT = 2;
    const static uint8_t G_TIMEZONE_NAME_LENGTH = 11;
    static char G_LED_STATUS[ 4 ]                               = "ON";               // for some reason, Home Assistant does not support setting a light color while it's off. So for MQTT integration, we must set status to ON


    static String g_unique_host_name( String _hostname ) {
      String mac_address = WiFi.macAddress().substring( 12 );
      mac_address.replace( ":", "" );
      return _hostname + "_" + mac_address;
    }
  #endif