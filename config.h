/*
 *
 * Twilio Breakout Massive SDK
 *
 * Copyright (c) 2019 Twilio, Inc.
 *
 * SPDX-License-Identifier:  Apache-2.0
 */

/**
 * \file config.h - defines configuration parameters. Can be modified by the user to adapt to a particular use-case
 *
 */
#ifndef BREAKOUT_CONFIG_H_
#define BREAKOUT_CONFIG_H_

// To enable an autentication mechanism, un-comment the #define below.  The following are supported mechanisms:
//
// USE_USERNAME_PASSWORD - Enables cleartext username/password authentication.
// USE_TLS_USERNAME_PASSWORD - Enables TLS and uses username/password for authentication
// USE_TLS_CLIENT_CERTIFICATES - Enables TLS and uses client certificates for authentication

//#define USE_USERNAME_PASSWORD
//#define USE_TLS_USERNAME_PASSWORD
//#define USE_TLS_CLIENT_CERTIFICATES

#define MQTT_BROKER_HOST "broker.hivemq.com"
// MQTT_BROKER_PORT generally is 1883 for clear-text, 8883 for TLS
#define MQTT_BROKER_PORT 1883
// Keep-Alive parameter for your connection. Depends on your network conditions,
//   20 is normally a safe bet
#define MQTT_KEEP_ALIVE 20

// Client ID uniquely identifies your device on the broker. If not unique, the broker will deny the connection
#define MQTT_CLIENT_ID "sourdough-bread-monitor"

// The topic to publish your data to
#define MQTT_PUBLISH_TOPIC "sourdough-bread-monitor/data"

// The topic to get 'sleep'/wakeup events from
#define MQTT_STATE_TOPIC "sourdough-bread-monitor/state"

#if defined(USE_USERNAME_PASSWORD) || defined(USE_TLS_USERNAME_PASSWORD)
// Login to use for login/password authentication
#define MQTT_LOGIN "sourdough-bread-monitor"

// Password to use for login/password authentication
#define MQTT_PASSWORD "sourdough-bread-monitor"
#endif

// Arduino loop interval
#define LOOP_INTERVAL 200

// MQTT Settings

// Set to 1 to enable MQTT client logging
#define MQTT_LOG_ENABLED 0

// Maximal message size
#define MAX_MQTT_PACKET_SIZE 500

// Maximal number of topics device can subscribe to
#define MAX_MQTT_SUBSCRIPTIONS 2

#if defined(USE_TLS_CLIENT_CERTIFICATES) || defined(USE_TLS_USERNAME_PASSWORD)
#define USE_TLS

// TLS_PROFILE_ID 0 is usually a good default unless using multiple profiles - possible values 0-4
#define TLS_PROFILE_ID 0

// Cipher to use. Refer to src/massive-sdk/src/modem/enumsRN4.h for the full list
#define TLS_CIPHER_SUITE USECPREF_CIPHER_SUITE_TLS_RSA_WITH_AES_256_CBC_SHA256

#include "tls_credentials.h"
#endif

#endif  // BREAKOUT_CONFIG_H_
