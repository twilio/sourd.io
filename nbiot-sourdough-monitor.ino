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

// we will send it every 5 min or so, 20 seconds for debugging though
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

    char commandText[512];
    snprintf(commandText, 512, "{\"value\":{\"humidity\":%4.2f,\"temp\":%4.2f,\"distance\":%ld}}", humidity,
             temperature, distance);
    if (!send_data(commandText,MQTT_HUMIDITY_TOPIC)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
  }
}
