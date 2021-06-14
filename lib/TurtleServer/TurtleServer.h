/* vim: set ft=c ts=2 sw=2 : */

/**
 * Library to handle the Minirobots turtle.
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


#ifndef TurtleServer_h
#define TurtleServer_h


#include <Arduino.h>

#include <Turtle.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>


class TurtleServer
{
  public:
    TurtleServer();

    void start(Turtle*);
    void handle();

    static String method();

    static void _logger(int);
    static void debug(String, String);

    static void setCORSHeaders();

    static void handleRoot();
    static void handleCORS();
    static void handleProgram();
    static void handleStatus();
    static void handleNotFound();
    static void handleDebug();
    static void handleWiFiReset();
    static void handleChangeMode();
};

#endif
