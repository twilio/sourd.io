#define BREAD_MONITOR

// https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
#include <DHT.h>

// Install https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger
#include <Ultrasonic.h>

// BEGIN CONFIGURATION SECTION

// Temp/humidity will be on D20 Grove port
#define SENSOR_PIN (D20)
#define DHTTYPE DHT11  // DHT 11

// Ultrasonic will be on D38 Grove port
#define ULTRASONIC_PIN 38

// we will send it every 20 seconds or so (this is probably high)
#define SEND_INTERVAL (20 * 1000)

// END CONFIGURATION SECTION

DHT dht(SENSOR_PIN, DHTTYPE);
Ultrasonic UltrasonicRanger(ULTRASONIC_PIN);

void bread_monitor_setup() {
  dht.begin();
  pinMode(ULTRASONIC_PIN, INPUT);
}

void bread_monitor_loop() {
  static unsigned long last_send = 0;

  if (!sleep && ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL))) {
    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();
    long distance     = UltrasonicRanger.MeasureInCentimeters();
    last_send = millis();

    char commandText[512];
    snprintf(commandText, 512, "{\"device\":\"%.*s\",\"humidity\":%4.2f,\"temp\":%4.2f,\"distance\":%ld}", imei.len, imei.s, humidity,
             temperature, distance);
    if (!send_data(commandText)) {
      LOG(L_WARN, "Error publishing message: (client connected status: %d)\r\n", mqtt->isConnected());
    }
  }
}
