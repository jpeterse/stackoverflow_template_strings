#pragma once
#ifndef MQTT_MESSAGE_TEMPLATE_H
  #define MQTT_MESSAGE_TEMPLATE_H

  #include "config.h"

  const char G_MQTT_SENSOR_CONFIG_TOPIC[] = "homeassistant/sensor/@@unique_name@@/config";
  const char G_MQTT_SENSOR_CONFIG[] = R"=====({"device_class":"illuminance","name":"Current","unique_id":"@@unique_name@@","state_class":"measurement","state_topic":"tele/clocks/@@unique_name@@","command_topic":"cmnd/clocks/@@unique_name@@","unit_of_measurement":"lx","value_template":"{{value_json.current_lux_level}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@","manufacturer":"Watt-This","model":"Wall Clock v@@hardware_version@@","sw_version":"@@firmware_version@@","configuration_url":"@@ip_address@@"}})=====";

  const char G_MQTT_SENSOR_AVG_CONFIG_TOPIC[] = "homeassistant/sensor/@@unique_name@@_avg/config";
  const char G_MQTT_SENSOR_AVG_CONFIG[] = R"=====({"device_class":"illuminance","name":"Average","unique_id":"@@unique_name@@_avg","state_class":"measurement","state_topic":"tele/clocks/@@unique_name@@","command_topic":"cmnd/clocks/@@unique_name@@","unit_of_measurement":"lx","value_template":"{{value_json.average_lux_level}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_SELECT_CONFIG_TOPIC[] = "homeassistant/select/@@unique_name@@_tz/config";
  const char G_MQTT_SELECT_CONFIG[] = R"=====({"entity_category":"config","name":"Timezone","unique_id":"@@unique_name@@_tz","state_topic":"stat/clocks/@@unique_name@@/config/tz","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{\"tz\": {{ this.attributes.options.index(value) }} }","options":[@@timezones@@],"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_NUMBER_DMD_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_dmd/config";
  const char G_MQTT_NUMBER_DMD_CONFIG[] = R"=====({"entity_category":"config","name":"LED Dimmed","unique_id":"@@unique_name@@_dmd","state_topic":"stat/clocks/@@unique_name@@/config/led_brightness_night","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{\"led_brightness_night\": {{value}} }","unit_of_measurement":"%","min":0,"max":100,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_NUMBER_LED_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_led/config";
  const char G_MQTT_NUMBER_LED_CONFIG[] = R"=====({"entity_category":"config","name":"LED Daytime","unique_id":"@@unique_name@@_led","state_topic":"stat/clocks/@@unique_name@@/config/led_brightness_day","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{\"led_brightness_day\": {{value}} }","unit_of_measurement":"%","min":0,"max":100,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_NUMBER_LXN_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_lxn/config";
  const char G_MQTT_NUMBER_LXN_CONFIG[] = R"=====({"entity_category":"config","name":"Light Level Nighttime","unique_id":"@@unique_name@@_lxn","state_topic":"stat/clocks/@@unique_name@@/config/light_level_night","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{\"light_level_night\": {{value}} }","unit_of_measurement":"lx","min":0,"max":65535,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_NUMBER_LXD_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_lxd/config";
  const char G_MQTT_NUMBER_LXD_CONFIG[] = R"=====({"entity_category":"config","name":"Light Level Daytime","unique_id":"@@unique_name@@_lxd","state_topic":"stat/clocks/@@unique_name@@/config/light_level_day","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{\"light_level_day\": {{value}} }","unit_of_measurement":"lx","min":0,"max":65535,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_LIGHT_R1B_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_r1b/config";
  const char G_MQTT_LIGHT_R1B_CONFIG[] = R"=====({"entity_category":"config","name":"Ring 1 Background","unique_id":"@@unique_name@@_r1b","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","payload_on":"{\"r1b\":\"on\"}","payload_off":"{\"r1b\":\"off\"}","supported_color_modes":["rgb","hs"],"color_mode":"rgb","color_mode_state_topic":"stat/clocks/@@unique_name@@/color_mode","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"r1b\":\"#{{'%02x%02x%02x'|format(red, green, blue)}}\"}","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb/r1b","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic":"cmnd/clocks/@@unique_name@@/brightness","brightness_command_template":"{\"r1b\":{{value}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";
//{"entity_category":"config","name":"Ring 1 Background","unique_id":"@@unique_name@@_r1b","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","supported_color_modes":["hs"],"color_mode":"hs","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic","cmnd/clocks/@@unique_name@@/brightness","brightness_command_template","{\"r1b\":{{value}}}"hs_command_topic":"cmnd/clocks/@@unique_name@@/hs","hs_command_template":"{\"r1b\":{{hue,sat}}}","hs_state_topic":"stat/clocks/@@unique_name@@/hs/r1b","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}}

  const char G_MQTT_LIGHT_R2B_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_r2b/config";
  const char G_MQTT_LIGHT_R2B_CONFIG[] = R"=====({"entity_category":"config","name":"Ring 2 Background","unique_id":"@@unique_name@@_r2b","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","payload_on":"{\"r2b\":\"on\"}","payload_off":"{\"r2b\":\"off\"}","supported_color_modes":["rgb","hs"],"color_mode":"rgb","color_mode_state_topic":"stat/clocks/@@unique_name@@/color_mode","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"r2b\":\"#{{'%02x%02x%02x'|format(red, green, blue)}}\"}","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb/r2b","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic":"cmnd/clocks/@@unique_name@@/brightness","brightness_command_template":"{\"r2b\":{{value}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_LIGHT_R3B_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_r3b/config";
  const char G_MQTT_LIGHT_R3B_CONFIG[] = R"=====({"entity_category":"config","name":"Ring 3 Background","unique_id":"@@unique_name@@_r3b","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","payload_on":"{\"r3b\":\"on\"}","payload_off":"{\"r3b\":\"off\"}","supported_color_modes":["rgb","hs"],"color_mode":"rgb","color_mode_state_topic":"stat/clocks/@@unique_name@@/color_mode","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"r3b\":\"#{{'%02x%02x%02x'|format(red, green, blue)}}\"}","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb/r3b","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic":"cmnd/clocks/@@unique_name@@/brightness","brightness_command_template":"{\"r3b\":{{value}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_LIGHT_R4B_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_r4b/config";
  const char G_MQTT_LIGHT_R4B_CONFIG[] = R"=====({"entity_category":"config","name":"Ring 4 Background","unique_id":"@@unique_name@@_r4b","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","payload_on":"{\"r4b\":\"on\"}","payload_off":"{\"r4b\":\"off\"}","supported_color_modes":["rgb","hs"],"color_mode":"rgb","color_mode_state_topic":"stat/clocks/@@unique_name@@/color_mode","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"r4b\":\"#{{'%02x%02x%02x'|format(red, green, blue)}}\"}","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb/r4b","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic":"cmnd/clocks/@@unique_name@@/brightness","brightness_command_template":"{\"r4b\":{{value}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_LIGHT_SEC_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_sec/config";
  const char G_MQTT_LIGHT_SEC_CONFIG[] = R"=====({"entity_category":"config","name":"Ring Section Marks","unique_id":"@@unique_name@@_sec","state_topic":"stat/clocks/@@unique_name@@/led","command_topic":"cmnd/clocks/@@unique_name@@","payload_on":"{\"sec\":\"on\"}","payload_off":"{\"sec\":\"off\"}","supported_color_modes":["rgb","hs"],"color_mode":"rgb","color_mode_state_topic":"stat/clocks/@@unique_name@@/color_mode","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"sec\":\"#{{'%02x%02x%02x'|format(red, green, blue)}}\"}","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb/sec","brightness_state_topic":"stat/clocks/@@unique_name@@/brightness","brightness_scale":100,"brightness_command_topic":"cmnd/clocks/@@unique_name@@/brightness","brightness_command_template":"{\"sec\":{{value}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

  const char G_MQTT_TELE_TOPIC[] = "tele/clocks/@@unique_name@@";
  const char G_MQTT_TELE[] = R"=====({"last_reading_dt":"@@date_time@@","current_lux_level":@@current_lux_level@@,"average_lux_level":@@average_lux_level@@})=====";

  const char G_MQTT_CMND_TOPIC[] = "cmnd/clocks/@@unique_name@@/#";

  const char G_MQTT_STAT_TZ_TOPIC[] = "stat/clocks/@@unique_name@@/config/tz";
  const char G_MQTT_STAT_LIGHT_LEVEL_NIGHT[] = "stat/clocks/@@unique_name@@/config/light_level_night";
  const char G_MQTT_STAT_LIGHT_LEVEL_DAY[] = "stat/clocks/@@unique_name@@/config/light_level_day";
  const char G_MQTT_STAT_LED_BRIGHTNESS_NIGHT[] = "stat/clocks/@@unique_name@@/config/led_brightness_night";
  const char G_MQTT_STAT_LED_BRIGHTNESS_DAY[] = "stat/clocks/@@unique_name@@/config/led_brightness_day";
  const char G_MQTT_STAT_LED_STATUS[] = "stat/clocks/@@unique_name@@/led";
  const char G_MQTT_STAT_LED_BRIGHTNESS[] = "stat/clocks/@@unique_name@@/led_brightness";
  const char G_MQTT_STAT_R1B_COLOR[] = "stat/clocks/@@unique_name@@/rgb/r1b";
  const char G_MQTT_STAT_R2B_COLOR[] = "stat/clocks/@@unique_name@@/rgb/r2b";
  const char G_MQTT_STAT_R3B_COLOR[] = "stat/clocks/@@unique_name@@/rgb/r3b";
  const char G_MQTT_STAT_R4B_COLOR[] = "stat/clocks/@@unique_name@@/rgb/r4b";
  const char G_MQTT_STAT_SEC_COLOR[] = "stat/clocks/@@unique_name@@/rgb/sec";
  
  struct DISCOVERY_MESSAGE_T {
    const char* TOPIC;
    const char* MESSAGE;
  };
  
  struct STAT_MESSAGE_T {
    const char* TOPIC;
    const G_CONFIGDATA CONFIG_KEY;
  };

  static const uint16_t G_MAX_MQTT_TOPIC_LENGTH = 100;
  static const uint16_t G_MAX_MQTT_MSG_LENGTH = 1151; // need to be increased to at least 1104 characters

  enum TMPL_STRINGS_E : uint8_t {
    KEY_ITERATER_FIRST,
    KEY_UNIQUE_NAME,
    KEY_DATE_TIME,
    KEY_CURRENT_LUX_LEVEL,
    KEY_AVERAGE_LUX_LEVEL,
    KEY_HARDWARE_VERSION,
    KEY_FIRMWARE_VERSION,
    KEY_IP_ADDRESS,
    KEY_TIMEZONE,
    KEY_LED_STATUS,
    KEY_UNKNOWN,
    KEY_ITERATE_LAST
  };

  static const uint8_t _keywordcount = 10;
  static char G_TEMPLATE_KEYWORDS[ _keywordcount ][ 21 ] { "first","unique_name","date_time","current_lux_level","average_lux_level","hardware_version","firmware_version","ip_address","timezones","last" };
  
  const DISCOVERY_MESSAGE_T G_DISCOVERY_MESSAGES[] = { 
    { &G_MQTT_SENSOR_CONFIG_TOPIC[0], &G_MQTT_SENSOR_CONFIG[0] },
    { &G_MQTT_SENSOR_AVG_CONFIG_TOPIC[0], &G_MQTT_SENSOR_AVG_CONFIG[0] },
    { &G_MQTT_SELECT_CONFIG_TOPIC[0], &G_MQTT_SELECT_CONFIG[0] },
    { &G_MQTT_NUMBER_DMD_CONFIG_TOPIC[0], &G_MQTT_NUMBER_DMD_CONFIG[0] },
    { &G_MQTT_NUMBER_LED_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LED_CONFIG[0] },
    { &G_MQTT_NUMBER_LXN_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LXN_CONFIG[0] },
    { &G_MQTT_NUMBER_LXD_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LXD_CONFIG[0] },
    { &G_MQTT_LIGHT_R1B_CONFIG_TOPIC[0], &G_MQTT_LIGHT_R1B_CONFIG[0] },
    { &G_MQTT_LIGHT_R2B_CONFIG_TOPIC[0], &G_MQTT_LIGHT_R2B_CONFIG[0] },
    { &G_MQTT_LIGHT_R3B_CONFIG_TOPIC[0], &G_MQTT_LIGHT_R3B_CONFIG[0] },
    { &G_MQTT_LIGHT_R4B_CONFIG_TOPIC[0], &G_MQTT_LIGHT_R4B_CONFIG[0] },
    { &G_MQTT_LIGHT_SEC_CONFIG_TOPIC[0], &G_MQTT_LIGHT_SEC_CONFIG[0] }
  };

  const STAT_MESSAGE_T G_STAT_MESSAGES[] = {
    { &G_MQTT_STAT_LED_STATUS[0], G_CONF_FIRMWARE_SEEED },  // Config data is not used for this message. G_CONF_FIRMWARE_SEEED is just to satisfy compiler
    { &G_MQTT_STAT_TZ_TOPIC[0], G_CONF_TIMEZONE },
    { &G_MQTT_STAT_LIGHT_LEVEL_NIGHT[0], G_CONF_LTR_MIN },
    { &G_MQTT_STAT_LIGHT_LEVEL_DAY[0], G_CONF_LTR_MAX },
    { &G_MQTT_STAT_LED_BRIGHTNESS_NIGHT[0], G_CONF_LED_BRIGHTNESS_MIN },
    { &G_MQTT_STAT_LED_BRIGHTNESS_DAY[0], G_CONF_LED_BRIGHTNESS_MAX },
    { &G_MQTT_STAT_LED_BRIGHTNESS[0], G_CONF_LED_BRIGHTNESS_MIN },  // set default brightness of leds to dimmed incandescent level
    { &G_MQTT_STAT_R1B_COLOR[0], G_CONF_RING_1_BACKGROUND_COLOR },
    { &G_MQTT_STAT_R2B_COLOR[0], G_CONF_RING_2_BACKGROUND_COLOR },
    { &G_MQTT_STAT_R3B_COLOR[0], G_CONF_RING_3_BACKGROUND_COLOR },
    { &G_MQTT_STAT_R4B_COLOR[0], G_CONF_RING_4_BACKGROUND_COLOR },
    { &G_MQTT_STAT_SEC_COLOR[0], G_CONF_RING_1_SECTION_MARKS }
  };

#endif