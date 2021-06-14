/* vim: set ft=c ts=2 sw=2 : */

/**
 * Minirobots config.
 *
 * https://gitlab.com/minirobots/turtle-robot-wifi
 *
 *
 * Author: Leo Vidarte <https://minirobots.com.ar>
 *
 * This is free software:
 * you can redistribute it and/or modify it
 * under the terms of the GPL version 3
 * as published by the Free Software Foundation.
 */

#include "Config.h"


// Functions

void logger(const char* a, bool nl)
{
  Serial.printf("%s%s", a, (nl ? "\n" : ""));
}
void logger(const char* a, const char* b, bool nl)
{
  Serial.printf("[ %-6s ] %s%s", a, b, (nl ? "\n" : ""));
}
void logger(const char* a, const char* b, const char* c, bool nl)
{
  Serial.printf("[ %-6s ] %s %s%s", a, b, c, (nl ? "\n" : ""));
}

/**
 * Returns esp8266 mac address.
 */
String getMAC ()
{
  uint8_t addr[6];
  WiFi.macAddress(addr);
  String mac;
  char buffer[17];
  sprintf(
    buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
    addr[0], addr[1], addr[2],
    addr[3], addr[4], addr[5]
  );
  mac = String(buffer);

  return mac;
}

/**
 * Returns mac address last 6 chars (the model part)
 *
 *  00:11:22:aa:bb:cc
 *   vendor   model
 *
 *  2C:3A:E8 Espressif Inc.
 */
String getID ()
{
  uint8_t addr[6];
  WiFi.macAddress(addr);
  String id;
  char buffer[6];
  sprintf(buffer, "%02x%02x%02x", addr[3], addr[4], addr[5]);
  id = String(buffer);

  return id;
}

/**
 * Returns the current IP.
 */
String getIP ()
{
  IPAddress addr = WiFi.localIP();
  String ip;

  for (int i = 0; i < 4; i++)
  {
    ip += String(addr[i]);
    if (i < 3) ip += String(".");
  }

  return ip;
}

