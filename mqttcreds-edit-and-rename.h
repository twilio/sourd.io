// Make a copy of this file and call it mqttcreds.h 
// Make sure it is in the same directory as your .ino

// This file tells your .ino which broker to publish to, and how to communicate with it
// You can use whatever broker you want! 
// I tested with Thingspeak and Adafruit, so I've included the info for using those here
// You can also create your own custom broker

/*** Picking A Broker ***/

// Adafruit docs: https://io.adafruit.com/api/docs/mqtt.html
// Set up Adafruit creds below and Uncomment this next line to use Adafruit:
    //  #define ADAFRUIT_MQTT
// Thingspeak docs: https://www.mathworks.com/help/thingspeak/mqtt-api.html
// Set up Thingspeak creds below and Uncomment this next line to use Thingspeak:
//    #define THINGSPEAK_MQTT
// Or do your own thing! Set up custom creds below and uncomment this next line to do something custom:
    #define CUSTOM_MQTT

/***-- CUSTOM BROKER --***/
#if defined(CUSTOM_MQTT)

    /*** To Auth, Or Not To Auth? ***/

    // If you're doing a custom broker, you may need different kinds of auth. 
    // To enable an autentication mechanism, un-comment the #define below.
    // The following are supported mechanisms:
    //
    // USE_USERNAME_PASSWORD - Enables cleartext username/password authentication.
    // USE_TLS_USERNAME_PASSWORD - Enables TLS and uses username/password for authentication
    // USE_TLS_CLIENT_CERTIFICATES - Enables TLS and uses client certificates for authentication

    #define USE_USERNAME_PASSWORD
    //#define USE_TLS_USERNAME_PASSWORD
    //#define USE_TLS_CLIENT_CERTIFICATES

    /*** My Broker ***/

    #define MQTT_BROKER_HOST "your.broker.here"

    // MQTT_BROKER_PORT generally is 1883 for clear-text, 8883 for TLS
    #define MQTT_BROKER_PORT 1883
    // Keep-Alive parameter for your connection. Depends on your network conditions,
    //   20 is normally a safe bet
    #define MQTT_KEEP_ALIVE 20

    /*** My Client Id ***/
    // Client ID uniquely identifies your device on the broker. If not unique, the broker will deny the connection
    #define MQTT_CLIENT_ID "your-unique-client-id"

    /*** My Topic ***/
    // The topic to publish your data to
    // You'll need, at the very least, a main topic and a state topic
    // You can customize how these are handled in the .ino file
    #define MQTT_PUBLISH_TOPIC "your-topic-name"
    #define MQTT_STATE_TOPIC "your-topic-name/state"

    /*** To Auth Or Not To Auth: The Sequel ***/
    // handles our auth from earlier
    #if defined(USE_USERNAME_PASSWORD) || defined(USE_TLS_USERNAME_PASSWORD)
    // Login to use for login/password authentication
    #define MQTT_LOGIN "YOUR-CREDS-HERE"
    // Password to use for login/password authentication
    #define MQTT_PASSWORD "YOUR-CREDS-HERE"
    #endif

#endif

/***-- ADAFRUIT.IO --***/
#if defined(ADAFRUIT_MQTT)

    // adafruit uses auth
    #define USE_USERNAME_PASSWORD
    // adafruit: "io.adafruit.com"
    #define MQTT_BROKER_HOST "io.adafruit.com"
    // MQTT_BROKER_PORT generally is 1883 for clear-text, 8883 for TLS
    #define MQTT_BROKER_PORT 1883
    // Keep-Alive parameter for your connection. Depends on your network conditions,
    //   20 is normally a safe bet
    #define MQTT_KEEP_ALIVE 20

    // adafruit client id: your username
    #define MQTT_CLIENT_ID "<your-adafruit-username>"

    // adafruit topic names
    // these are the adafruit ones for a group called "breadmonitor"
    #define MQTT_PUBLISH_TOPIC "<your-adafruit-username>/feeds/breadmonitor/json"
    #define MQTT_HUMIDITY_TOPIC "<your-adafruit-username>/feeds/breadmonitor.humidity/json"
    #define MQTT_TEMP_TOPIC "<your-adafruit-username>/feeds/breadmonitor.temp/json"
    #define MQTT_DISTANCE_TOPIC "<your-adafruit-username>/feeds/breadmonitor.distance/json"
    #define MQTT_STATE_TOPIC "<your-adafruit-username>/feeds/breadmonitor/state"

    #if defined(USE_USERNAME_PASSWORD) || defined(USE_TLS_USERNAME_PASSWORD)
    // Login to use for login/password authentication
    // adafruit login: your username
    #define MQTT_LOGIN "<your-adafruit-username>"
    // adafruit password: your API key
    #define MQTT_PASSWORD "<your-adafruit-api-key>"
    #endif

#endif

/***-- THINGSPEAK --***/
#if defined(THINGSPEAK_MQTT)

    // thingspeak uses auth
    #define USE_USERNAME_PASSWORD
    // thingspeak: "mqtt.thingspeak.com"
    #define MQTT_BROKER_HOST "mqtt.thingspeak.com"
    // MQTT_BROKER_PORT generally is 1883 for clear-text, 8883 for TLS
    #define MQTT_BROKER_PORT 1883
    // Keep-Alive parameter for your connection. Depends on your network conditions,
    //   20 is normally a safe bet
    #define MQTT_KEEP_ALIVE 20

    // thingspeak client id: generate a random unique client ID
    //     you can use something like https://www.uuidgenerator.net/ to get one
    #define MQTT_CLIENT_ID "<your-randomly-generated-ID>"

    // thingspeak formatting looks like this for topics
    // thingspeak topic name for channel feeds: "channels/<channelID>/publish/<apikey>"
    //     where <apikey> is the API key of that specific channel
    // thingspeak topic name for channel fields: "channels/<channelID>/publish/fields/field<fieldnumber>/<apikey>"
    //     where <apikey> is the API key of that specific channel
    //     and <fieldnumber> is the number of the field you want to update

    // So our thingspeak topic names look like this
    #define MQTT_PUBLISH_TOPIC "channels/<channelID>/publish/<channel-API-key>"
    #define MQTT_STATE_TOPIC "channels/<channelID>/publish/fields/field4/<channel-API-key>"

    #if defined(USE_USERNAME_PASSWORD) || defined(USE_TLS_USERNAME_PASSWORD)
    // Login to use for login/password authentication
    // thingspeak login: any username is fine, doesn't have to be your actual
    #define MQTT_LOGIN "<any-random-username>"
    // thingspeak password: your API key
    #define MQTT_PASSWORD "<your-thingspeak-account-api-key>"
    #endif

#endif
