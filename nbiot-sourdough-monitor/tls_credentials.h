/*
 *
 * Twilio Breakout Massive SDK
 *
 * Copyright (c) 2019 Twilio, Inc.
 *
 * SPDX-License-Identifier:  Apache-2.0
 */

/**
 * \file tls_credentials.h - TLS credentials to use in your application.
 *
 */

#ifndef TLS_CREDENTIALS_H_
#define TLS_CREDENTIALS_H_

// You can provide either ASCII PEM encoded certificates and a private key below, or hex escaped
// DER encoded items.  The benefit of providing ASCII PEM is ease to copy/paste in and readability.
// The benefit of providing hex escaped DER items is less flash storage required and no runtime
// base64 decoding when comparing the MD5 sums before writing to the module.  In either case, choose
// the section below best for your needs and remove the comment designators.


// To provide PEM encoded items, use the openssl command line or another tool to convert, if needed,
// the trusted CA root, device cert and private key and paste below - observing the required header/footer
// and ""\ format.

//#define TLS_SERVER_CA "" \
//"-----BEGIN CERTIFICATE-----" \
//"..." \
//"-----END CERTIFICATE-----"
//#endif

#ifdef USE_TLS_CLIENT_CERTIFICATES
//#define TLS_DEVICE_CERT "" \
//"-----BEGIN CERTIFICATE-----" \
//"..." \
//"-----END CERTIFICATE-----"

//#define TLS_DEVICE_PKEY "" \
//"-----BEGIN RSA PRIVATE KEY-----" \
//"..." \
//"-----END RSA PRIVATE KEY-----"
#endif


// To provide DER encoded items, a help script is provided.  To generate this file from *.pem or *.der
// files, use the script in the 'scripts' directory of the library or
// https://github.com/twilio/Breakout_Massive_SDK_Arduino/scripts/ :
//
//   ./make_tls_credentials.sh cert.pem pkey.pem ca.pem
//
// And copy/paste the results below, see the commented out lines with \x00\x01\x02...

//#define TLS_SERVER_CA "\x00\x01\x02..."

#ifdef USE_TLS_CLIENT_CERTIFICATES
//#define TLS_DEVICE_CERT "\x00\x01\x02..."

//#define TLS_DEVICE_PKEY "\x00\x01\x02.."
#endif

#endif  // TLS_CREDENTIALS_H_
