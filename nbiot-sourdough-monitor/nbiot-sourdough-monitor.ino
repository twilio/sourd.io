// Tell it to include the important stuff from this sketch
#include "config.h"
#include "mqttcreds.h"
#include "modem.h"
#include "mqtt.h"

/**** Libraries for the bread monitor ****/

// We also need to include libraries for the temperature and humidity sensor, as well as the ultrasonic sensor
// https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
#include <DHT.h>
// Install via zip if not working: https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger
#include <Ultrasonic.h>

/**** Variables for the bread monitor ****/

// Temp/humidity will be on D20 Grove port
#define SENSOR_PIN (D20)
#define DHTTYPE DHT11  // DHT 11
DHT dht(SENSOR_PIN, DHTTYPE);

// Ultrasonic will be on D38 Grove port
#define ULTRASONIC_PIN 38
Ultrasonic UltrasonicRanger(ULTRASONIC_PIN);

// we will send it every 20 second or so
#define SEND_INTERVAL (20 * 1000)


/**** Code to run once, when the device boots up ****/

void setup() {

  // run code to set up mqtt and modem stuff
  
  owl_log_set_level(L_INFO);

  modem_setup();
  mqtt_setup();

  // then, tell the serial log that we are going to start

  LOG(L_WARN, "Hi! I am the bread monitor. Now trying to start....\r\n");

  // run our bread monitor setup...
  LOG(L_WARN, "Powering on bread sensors....\r\n");
  dht.begin();
  pinMode(ULTRASONIC_PIN, INPUT);
  
  return;
}

/**** Code to run repeatedly, after the device boots ****/

void loop() {

// If you aren't connected yet, keep trying to connect!
  if (!mqtt_loop()) {
    goto continue_loop;
  }

// also, run the bread monitor loop
  bread_monitor_loop();

// This is the loop you'll run if you aren't connected yet
continue_loop:
  rn4_modem->AT.spin();
  if (mqtt != nullptr) {
    mqtt->yield(LOOP_INTERVAL);
  }
}


void bread_monitor_loop() {
  static unsigned long last_send = 0;

  if (!sleep && ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL))) {
    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();
    long distance     = UltrasonicRanger.MeasureInCentimeters();
    last_send = millis();

    #if defined(CUSTOM_MQTT)
    // sending data to a custom MQTT broker. Edit this to work with your broker!
    // this part will only be compiled if we are pubishing to a custom broker! (see mqttcreds-edit-and-rename.h)
    char commandText[512];
    // edit the below text to fit your broker's needs!
    snprintf(commandText, 512, "{\"device\":\"%.*s\",\"humidity\":%4.2f,\"temp\":%4.2f,\"distance\":%ld}", imei.len, imei.s, humidity,temperature, distance);
    if (!send_data(commandText)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
    #endif

    #if defined(ADAFRUIT_MQTT)
    // sending data to Adafruit.io feeds, using Adafruit's json formatting
    // this part will only be compiled if we are pubishing to Adafruit! (see mqttcreds-edit-and-rename.h)
    char humidityText[512];
    char tempText[512];
    char distanceText[512];
    snprintf(humidityText, 512, "{\"value\":%4.2f}", humidity);
    snprintf(tempText,512, "{\"value\":%4.2f}",temperature);
    snprintf(distanceText,512, "{\"value\":%ld}",distance);
    if (!send_data(humidityText,MQTT_HUMIDITY_TOPIC)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
    if (!send_data(tempText,MQTT_TEMP_TOPIC)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
    if (!send_data(distanceText,MQTT_DISTANCE_TOPIC)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
    #endif

    #if defined(THINGSPEAK_MQTT)
    // sending individually to Thingspeak channels, using Thingspeak query formatting
    // this part will only be compiled if we are pubishing to Thingspeak! (see mqttcreds-edit-and-rename.h)
    char pubText[512];
    snprintf(pubText, 512, "field1=%4.2f&field2=%4.2f&field3=%ld&status=MQTTPUBLISH", humidity, temperature, distance);
    if (!send_data(pubText,MQTT_PUBLISH_TOPIC)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
    #endif

  }
}
