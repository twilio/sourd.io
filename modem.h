// Include the BreakoutSDK which includes functionality for the Alfa developer
// board and wrapping of the u-blox SARA-N410-02B cellular module
#include <BreakoutSDK.h>
#include <platform/ArduinoSeeedOwlSerial.h>

OwlModemRN4 *rn4_modem                 = nullptr;
ArduinoSeeedUSBOwlSerial *debug_serial = nullptr;
ArduinoSeeedHwOwlSerial *modem_serial  = nullptr;
ArduinoSeeedHwOwlSerial *gnss_serial   = nullptr;

str imei = {.s = nullptr, .len = 0};

// forward declarations
void configure_tls();
void fail();

void modem_setup() {
  
  // Feel free to change the log verbosity. E.g. from most critical to most verbose:
  //   - errors: L_ALERT, L_CRIT, L_ERR, L_ISSUE
  //   - warnings: L_WARN, L_NOTICE
  //   - information & debug: L_INFO, L_DB, L_DBG, L_MEM
  // When logging, the additional L_CLI level ensure that the output will always be visible, no matter the set level.
  LOG(L_WARN, "Arduino setup() starting up\r\n");
  debug_serial = new ArduinoSeeedUSBOwlSerial(&SerialDebugPort);
  modem_serial = new ArduinoSeeedHwOwlSerial(&SerialModule, SerialModule_Baudrate);
  rn4_modem    = new OwlModemRN4(modem_serial, debug_serial, gnss_serial);

  LOG(L_WARN, "Powering on module...");
  if (!rn4_modem->powerOn()) {
    LOG(L_WARN, "... error powering on.\r\n");
    fail();
  }
  LOG(L_WARN, "... done powering on.\r\n");

  LOG(L_WARN, "Initializing the module and registering on the network...");
  char *cops = nullptr;
#ifdef MOBILE_OPERATOR
  cops = MOBILE_OPERATOR;
#endif

  at_cops_format_e cops_format = AT_COPS__Format__Numeric;

#ifdef MOBILE_OPERATOR_FORMAT
  cops_format = MOBILE_OPERATOR_FORMAT;
#endif
  if (!rn4_modem->initModem(TESTING_VARIANT_INIT, TESTING_APN, cops, cops_format)) {
    LOG(L_WARN, "... error initializing.\r\n");
    fail();
  }
  LOG(L_WARN, "... done initializing.\r\n");

  str imei_temp = {.s = nullptr, .len = 0};
  rn4_modem->information.getIMEI(&imei_temp);
  str_dup(imei, imei_temp);

  LOG(L_WARN, "Waiting for network registration...");
  if (!rn4_modem->waitForNetworkRegistration("Telemetry", TESTING_VARIANT_REG)) {
    LOG(L_WARN, "... error registering on the network.\r\n");
    fail();
  }
  LOG(L_WARN, "... done waiting.\r\n");

#ifdef USE_TLS
  configure_tls();
#endif
}

#ifdef USE_TLS
void configure_tls() {
#if defined(TLS_SERVER_CA)
  str ca = {.s = TLS_SERVER_CA, .len = sizeof(TLS_SERVER_CA)};
  rn4_modem->ssl.setServerCA(ca);
#endif

#if defined(TLS_DEVICE_CERT) && defined(TLS_DEVICE_PKEY)
  str cert = {.s = TLS_DEVICE_CERT, .len = sizeof(TLS_DEVICE_CERT)};
  rn4_modem->ssl.setDeviceCert(cert);

  str key = {.s = TLS_DEVICE_PKEY, .len = sizeof(TLS_DEVICE_PKEY)};
  rn4_modem->ssl.setDevicePkey(key);
#endif

  rn4_modem->ssl.initContext(TLS_PROFILE_ID, TLS_CIPHER_SUITE);
}
#endif

void fail() {
  LOG(L_ERR, "Initialization failed, entering bypass mode. Reset the board to start again.\r\n");

  while (true) {
    if (SerialDebugPort.available()) {
      int bt = SerialDebugPort.read();
      SerialModule.write(bt);
    }

    if (SerialModule.available()) {
      int bt = SerialModule.read();
      SerialDebugPort.write(bt);
    }
  }
}
