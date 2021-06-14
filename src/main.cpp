/* vim: set ft=c ts=2 sw=2 : */

/**
 * Minirobots turtle.
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

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino

// Needed for WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
//
#include <Ticker.h>
//
#include <Config.h>
#include <TurtleServer.h>
#include <Turtle.h>
#include <Pixel.h>

#include <EEPROM.h>

const char *ssid = "MiniRobot Turtle";

TurtleServer server;
Turtle turtle;
Ticker ticker;

void setup()
{
  Serial.begin(115200);

  // Init server
  logger("");
  logger("");
  String minirobotsR = R"=====(
   /'\_/`\  __          __             /\ \            /\ \__
  /\      \/\_\    ___ /\_\  _ __   ___\ \ \____    ___\ \ ,_\   ____
  \ \ \__\ \/\ \ /' _ `\/\ \/\`'__\/ __`\ \ '__`\  / __`\ \ \/  /',__\
   \ \ \_/\ \ \ \/\ \/\ \ \ \ \ \//\ \L\ \ \ \L\ \/\ \L\ \ \ \_/\__, `\
    \ \_\\ \_\ \_\ \_\ \_\ \_\ \_\\ \____/\ \_,__/\ \____/\ \__\/\____/
     \/_/ \/_/\/_/\/_/\/_/\/_/\/_/ \/___/  \/___/  \/___/  \/__/\/___/
  )=====";
  Serial.println(minirobotsR);
  logger(" init ", "Minirobots starting...");

  turtle.begin();
  Pixel::begin();
  Pixel::setAll(127, 127, 127);

  String mac = getMAC();
  logger(" init ", "MAC Address:", mac.c_str());

  EEPROM.begin(512);
  char appMode = EEPROM.read(0);
  Serial.print("APP MOD: ");
  Serial.println(appMode);

  if (appMode == 'f')
  {
    Serial.println("NORMAL MODE");
    String conn_msg = String("Connecting to WiFi using ") + String(WiFi.SSID()) + String("...");
    logger(" init ", conn_msg.c_str(), false);
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(false);
    wifiManager.autoConnect(turtle.id().c_str());
    logger("Connected!");

    String ip = getIP();
    logger(" init ", "IP Address:", ip.c_str());

    ticker.attach(0.2, [](){ Pixel::toggleCross(0, 0, 63); });
    turtle.connect();
    turtle.update();
    ticker.detach();
  }
  else
  {
    Serial.println("APP MODE");
    WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  }

  Pixel::setAll(0, 0, 127);
  server.start(&turtle);
}

void loop()
{
  server.handle();

  if (!queue.isLock())
  {
    turtle.handle();
    delay(3);
    yield();
  }
}
