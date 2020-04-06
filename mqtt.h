// https://github.com/monstrenyatko/ArduinoMqtt
#include <MqttClient.h>

// A Paho IP Stack implementation compatible with BreakoutSDK
#include <massive-sdk/src/shims/paho-mqtt/RN4PahoIPStack.h>

RN4PahoIPStack *ip_stack                                                   = nullptr;
static MqttClient *mqtt = NULL;

bool sleep = false;

// forward declarations
bool mqtt_connect();
static void device_state_callback(MqttClient::MessageData &message);

// ============== Object to supply system functions ============================
class System: public MqttClient::System {
public:

  unsigned long millis() const {
    return ::millis();
  }
};

class OwlLog {
  public:
    OwlLog() {}
    ~OwlLog() {}
    void println(const char *buf) {
        LOG(L_INFO, "%s\r\n", buf);
    }
};

static OwlLog *owlLog = NULL;

void mqtt_setup() {
  ip_stack = new RN4PahoIPStack(&rn4_modem->socket);
  owlLog = new OwlLog;

  // Setup MqttClient
  MqttClient::System *mqttSystem = new System;
  MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<OwlLog>(*owlLog);
  MqttClient::Network * mqttNetwork = new MqttClient::NetworkImpl<RN4PahoIPStack>(*ip_stack, *mqttSystem);
  // Make 512 bytes send buffer
  MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<MAX_MQTT_PACKET_SIZE>();
  // Make 512 bytes receive buffer
  MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<MAX_MQTT_PACKET_SIZE>();
  // Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<MAX_MQTT_SUBSCRIPTIONS>();
  // Configure client options
  MqttClient::Options mqttOptions;
  // Set command timeout to 10 seconds
  mqttOptions.commandTimeoutMs = 10000;
  // Make client object
  mqtt = new MqttClient(
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers
  );
}

bool mqtt_loop() {
  if (!ip_stack->connected()) {
    if (mqtt->isConnected()) {
      mqtt->disconnect();
    }
    if (mqtt_connect() != 1) {
      LOG(L_WARN, "Reconnection failed\r\n");
    }
    delay(LOOP_INTERVAL);
    return false;
  }
  if (!mqtt->isConnected()) {
    ip_stack->disconnect();
    return false;
  }
  return true;
}

bool mqtt_connect() {
#ifdef USE_TLS
  if (!ip_stack->connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, true, TLS_PROFILE_ID)) {
#else
  if (!ip_stack->connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, false)) {
#endif
    LOG(L_WARN, "Failed to establish connection to the broker\r\n");
    return false;
  }

  // Start new MQTT connection
  MqttClient::ConnectResult connectResult;
  // Connect
  {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 4;
    options.clientID.cstring = (char*)MQTT_CLIENT_ID;
    options.cleansession = true;
#ifdef MQTT_LOGIN
    options.username.cstring = MQTT_LOGIN;
#endif
#ifdef MQTT_PASSWORD
    options.password.cstring = MQTT_PASSWORD;
#endif

    if (MQTT_KEEP_ALIVE > 0) {
      options.keepAliveInterval = MQTT_KEEP_ALIVE;
    }
    MqttClient::Error::type rc = mqtt->connect(options, connectResult);
    if (rc != MqttClient::Error::SUCCESS) {
      LOG(L_WARN, "Failed to connect to MQTT broker, error: %i\r\n", rc);
      return false;
    }
  }

  MqttClient::Error::type rc = mqtt->subscribe(MQTT_STATE_TOPIC, MqttClient::QOS0, device_state_callback);
  if (rc != MqttClient::Error::SUCCESS) {
    LOG(L_WARN, "Failed to subscribe to \"state\" topic: %i\r\n", rc);
    return false;
  }

  return true;
}

bool send_data(char *data) {
  MqttClient::Message message;
  message.qos        = MqttClient::QOS0;
  message.retained   = false;
  message.dup        = false;
  message.payload    = data;
  message.payloadLen = strlen(data);
  return (mqtt->publish(MQTT_PUBLISH_TOPIC, message) == 0);
}

void mqtt_disconnect() {
  LOG(L_INFO, "Disconnecting mqtt client and ip stack\r\n");
  if (mqtt) {
    mqtt->disconnect();
    mqtt = nullptr;
  }

  if (ip_stack) {
    ip_stack->disconnect();
  }
}

static void device_state_callback(MqttClient::MessageData &message) {
  str message_str = {.s = (char *)message.message.payload, .len = message.message.payloadLen};
  if (str_equal_char(message_str, "sleep")) {
    if (sleep) {
      LOG(L_INFO, "Already sleeping\r\n");
    } else {
      LOG(L_INFO, "Going to sleep\r\n");
      sleep = true;
    }
    return;
  }

  if (str_equal_char(message_str, "wakeup")) {
    if (!sleep) {
      LOG(L_INFO, "Already awake\r\n");
    } else {
      LOG(L_INFO, "Waking up\r\n");
      sleep = false;
    }
    return;
  }

  LOG(L_WARN, "Unknown state: %.*s\r\n", message_str.len, message_str.s);
}
