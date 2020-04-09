
// To enable an autentication mechanism, un-comment the #define below.  The following are supported mechanisms:
//
// USE_USERNAME_PASSWORD - Enables cleartext username/password authentication.
// USE_TLS_USERNAME_PASSWORD - Enables TLS and uses username/password for authentication
// USE_TLS_CLIENT_CERTIFICATES - Enables TLS and uses client certificates for authentication

#define USE_USERNAME_PASSWORD
//#define USE_TLS_USERNAME_PASSWORD
//#define USE_TLS_CLIENT_CERTIFICATES

#define MQTT_BROKER_HOST "your.broker.here"
// adafruit: "io.adafruit.com"

// MQTT_BROKER_PORT generally is 1883 for clear-text, 8883 for TLS
#define MQTT_BROKER_PORT 1883
// Keep-Alive parameter for your connection. Depends on your network conditions,
//   20 is normally a safe bet
#define MQTT_KEEP_ALIVE 20

// Client ID uniquely identifies your device on the broker. If not unique, the broker will deny the connection
#define MQTT_CLIENT_ID "sourdough-bread-monitor"
// adafruit client id: your username

// The topic to publish your data to. This is the "catch all" topic
#define MQTT_PUBLISH_TOPIC "sourdough-bread-monitor/data"
// adafruit topic name: "your-username-here/feeds/breadmonitor/json"

// subtopics for publishing specific humidity, temperature, and distance data
// these are the adafruit ones for making groups
#define MQTT_HUMIDITY_TOPIC "your-username-here/g/breadmonitor.humidity/json"
#define MQTT_TEMP_TOPIC "your-username-here/g/breadmonitor.temp/json"
#define MQTT_DISTANCE_TOPIC "your-username-here/g/breadmonitor.distance/json"

// The topic to get 'sleep'/wakeup events from
#define MQTT_STATE_TOPIC "sourdough-bread-monitor/state"

#if defined(USE_USERNAME_PASSWORD) || defined(USE_TLS_USERNAME_PASSWORD)
// Login to use for login/password authentication
#define MQTT_LOGIN "YOUR-CREDS-HERE"

// Password to use for login/password authentication
#define MQTT_PASSWORD "YOUR-CREDS-HERE"
#endif
