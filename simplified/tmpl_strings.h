const char tmplt_title_1[] = "@@first@@ line";
const char tmplt_str_1[] = "Lorem ipsum dolor @@replace@@ consectetur adipiscing elit.";
const char tmplt_title_2[] = "@@second@@ line";
const char tmplt_str_2[] = "Pellentesque consectetur @@me@@ dui sollicitudin nec.";
const char tmplt_title_3[] = "@@third@@ line";
const char tmplt_str_3[] = "Vivamus porttitor @@and me@@ cursus.";
const char G_MQTT_SENSOR_CONFIG_TOPIC[] = "homeassistant/sensor/@@unique_name@@/config";
const char G_MQTT_SENSOR_CONFIG[] = R"=====({"device_class":"illuminance","name":"Current","unique_id":"@@unique_name@@","state_class":"measurement","state_topic":"tele/clocks/@@unique_name@@","command_topic":"cmnd/clocks/@@unique_name@@","unit_of_measurement":"lx","value_template":"{{value_json.current_lux_level}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@","manufacturer":"Watt-This","model":"Wall Clock v@@hardware_version@@","sw_version":"@@firmware_version@@","configuration_url":"@@ip_address@@"}})=====";

const char G_MQTT_SENSOR_AVG_CONFIG_TOPIC[] = "homeassistant/sensor/@@unique_name@@_avg/config";
const char G_MQTT_SENSOR_AVG_CONFIG[] = R"=====({"device_class":"illuminance","name":"Average","unique_id":"@@unique_name@@_avg","state_class":"measurement","state_topic":"tele/clocks/@@unique_name@@","command_topic":"cmnd/clocks/@@unique_name@@","unit_of_measurement":"lx","value_template":"{{value_json.average_lux_level}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_SELECT_CONFIG_TOPIC[] = "homeassistant/select/@@unique_name@@_tz/config";
const char G_MQTT_SELECT_CONFIG[] = R"=====({"entity_category":"config","name":"Timezone","unique_id":"@@unique_name@@_tz","state_topic":"stat/clocks/@@unique_name@@/tz","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{"tz": {{ this.attributes.options.index(value) }} }","options":[@@timezones@@],"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_NUMBER_DMD_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_dmd/config";
const char G_MQTT_NUMBER_DMD_CONFIG[] = R"=====({"entity_category":"config","name":"LED Dimmed","unique_id":"@@unique_name@@_dmd","state_topic":"stat/clocks/@@unique_name@@/led_brightness_night","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{"led_brightness_night": {{ this.value }} }","unit_of_measurement":"%","min":0,"max":100,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_NUMBER_LED_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_led/config";
const char G_MQTT_NUMBER_LED_CONFIG[] = R"=====({"entity_category":"config","name":"LED Daytime","unique_id":"@@unique_name@@_led","state_topic":"stat/clocks/@@unique_name@@/led_brightness_day","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{"led_brightness_day": {{ this.value }} }","unit_of_measurement":"%","min":0,"max":100,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_NUMBER_LXN_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_lxn/config";
const char G_MQTT_NUMBER_LXN_CONFIG[] = R"=====({"entity_category":"config","name":"Light Level Nighttime","unique_id":"@@unique_name@@_lxn","state_topic":"stat/clocks/@@unique_name@@/light_level_night","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{"light_level_night": {{ this.value }} }","unit_of_measurement":"lx","min":0,"max":65535,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_NUMBER_LXD_CONFIG_TOPIC[] = "homeassistant/number/@@unique_name@@_lxd/config";
const char G_MQTT_NUMBER_LXD_CONFIG[] = R"=====({"entity_category":"config","name":"Light Level Daytime","unique_id":"@@unique_name@@_lxd","state_topic":"stat/clocks/@@unique_name@@/light_level_day","command_topic":"cmnd/clocks/@@unique_name@@","command_template":"{"light_level_day": {{ this.value }} }","unit_of_measurement":"lx","min":0,"max":65535,"device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_LIGHT_R1B_CONFIG_TOPIC[] = "homeassistant/light/@@unique_name@@_r1b/config";
const char G_MQTT_LIGHT_R1B_CONFIG[] = R"=====({"entity_category":"config","name":"Ring 1 Background","unique_id":"@@unique_name@@","state_topic":"stat/clocks/@@unique_name@@/r1b","command_topic":"cmnd/clocks/@@unique_name@@","supported_color_modes":["rgb","hs"],"color_mode":"hs","rgb_command_topic":"cmnd/clocks/@@unique_name@@/rgb","rgb_state_topic":"stat/clocks/@@unique_name@@/rgb","rgb_command_template":"{\"r1b\":{{red,green,blue}}}","hs_command_topic":"cmnd/clocks/@@unique_name@@/hs","hs_state_topic":"stat/clocks/@@unique_name@@/hs","hs_command_template":"{\"r1b\":{{hue,sat}}}","device":{"name":"@@unique_name@@","identifiers":"@@unique_name@@"}})=====";

const char G_MQTT_TELE_TOPIC[] = "tele/clocks/@@unique_name@@";
const char G_MQTT_TELE[] = R"=====({"last_reading_dt":"@@data_time@@","current_lux_level":@@current_light_level@@,"average_lux_level":@@average_light_level@@})=====";

const char G_MQTT_STAT_TZ_TOPIC[] = "stat/clocks/@@unique_name@@/tz";
const char G_MQTT_STAT_LIGHT_LEVEL_NIGHT[] = "stat/clocks/@@unique_name@@/light_level_night";
const char G_MQTT_STAT_LIGHT_LEVEL_DAY[] = "stat/clocks/@@unique_name@@/light_level_day";


struct tmplt_str_t {
  const char* title;
  const char* msg;
};


const tmplt_str_t tmplt_strs[] = {
  { &G_MQTT_SENSOR_CONFIG_TOPIC[0], &G_MQTT_SENSOR_CONFIG[0] },
  { &G_MQTT_SENSOR_AVG_CONFIG_TOPIC[0], &G_MQTT_SENSOR_AVG_CONFIG[0] },
  { &G_MQTT_SELECT_CONFIG_TOPIC[0], &G_MQTT_SELECT_CONFIG[0] },
  { &G_MQTT_NUMBER_DMD_CONFIG_TOPIC[0], &G_MQTT_NUMBER_DMD_CONFIG[0] },
  { &G_MQTT_NUMBER_LED_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LED_CONFIG[0] },
  { &G_MQTT_NUMBER_LXN_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LXN_CONFIG[0] },
  { &G_MQTT_NUMBER_LXD_CONFIG_TOPIC[0], &G_MQTT_NUMBER_LXD_CONFIG[0] },
  { &G_MQTT_LIGHT_R1B_CONFIG_TOPIC[0], &G_MQTT_LIGHT_R1B_CONFIG[0] }
};
