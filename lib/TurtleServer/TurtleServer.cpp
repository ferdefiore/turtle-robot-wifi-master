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


#include "Config.h"
#include "TurtleServer.h"

#include <EEPROM.h>


ESP8266WebServer _server(80);


boolean _debug = false;
Turtle* _turtle;


TurtleServer::TurtleServer ()
{
}

void TurtleServer::start (Turtle* turtle)
{
  // Reference to Turtle
  _turtle = turtle;

  // Routes
  _server.on("/",             HTTP_GET,     TurtleServer::handleRoot);
  _server.on("/",             HTTP_OPTIONS, TurtleServer::handleCORS);
  _server.on("/program",      HTTP_POST,    TurtleServer::handleProgram);
  _server.on("/program",      HTTP_OPTIONS, TurtleServer::handleCORS);
  _server.on("/status",       HTTP_GET,     TurtleServer::handleStatus);
  _server.on("/status",       HTTP_OPTIONS, TurtleServer::handleCORS);
  _server.on("/debug",        HTTP_POST,    TurtleServer::handleDebug);
  _server.on("/debug",        HTTP_OPTIONS, TurtleServer::handleCORS);
  _server.on("/wifi-reset",   HTTP_POST,    TurtleServer::handleWiFiReset);
  _server.on("/wifi-reset",   HTTP_OPTIONS, TurtleServer::handleCORS);
  _server.on("/change-mode",  HTTP_POST,    TurtleServer::handleChangeMode);
  _server.on("/change-mode",  HTTP_OPTIONS, TurtleServer::handleCORS);
  //
  // 404 - Not Found
  _server.onNotFound(TurtleServer::handleNotFound);

  if (MDNS.begin(_turtle->id().c_str())) {
    logger("server",
      String("Multicast DNS responder started for " + _turtle->id() + ".local").c_str()
    );
  }

  _server.begin();
  logger("server", "Minirobots Turtle API started on port 80");
}

void TurtleServer::handle ()
{
  _server.handleClient();
}

/** Static methods **/

String TurtleServer::method ()
{
  String method = "";

  if      (_server.method() == HTTP_GET    ) method = "GET";
  else if (_server.method() == HTTP_POST   ) method = "POST";
  else if (_server.method() == HTTP_OPTIONS) method = "OPTIONS";

  return method;
}

void TurtleServer::_logger (int status)
{
  String method = TurtleServer::method();
  String uri = _server.uri();

  logger("server", String(method + " " + uri).c_str(), String(status).c_str());
}

void TurtleServer::debug (String s0, String s1)
{
  if (_debug) logger("debug", s0.c_str(), s1.c_str());
}

void TurtleServer::setCORSHeaders ()
{
  _server.sendHeader("Access-Control-Allow-Origin" , "*");
  _server.sendHeader("Access-Control-Allow-Headers", "X-Requested-with, Content-Type");
  _server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
}

void TurtleServer::handleRoot ()
{
  setCORSHeaders();
  String json = "{"
    "\"message\": \"Welcome to Minirobots Turtle API! :)\", "
    "\"robot\": {"
      "\"id\": \""       + _turtle->id() + "\", "
      "\"ssid\": \""     + WiFi.SSID()   + "\", "
      "\"ip\": \""       + getIP()       + "\", "
      "\"mac\": \""      + getMAC()      + "\", "
      "\"firmware\": \"" + VERSION       + "\", "
      "\"config\": {"
        "\"pen_up\": "           + _turtle->config("pen_up")           + ", "
        "\"pen_down\": "         + _turtle->config("pen_down")         + ", "
        "\"steps_per_degree\": " + _turtle->config("steps_per_degree") + ", "
        "\"steps_per_mm\": "     + _turtle->config("steps_per_mm")     + ""
      "}"
    "}"
  "}";

  _server.send(200, "application/json", json);
  TurtleServer::_logger(200);
}

void TurtleServer::handleCORS ()
{
  setCORSHeaders();
  _server.send(200, "application/json");
  TurtleServer::_logger(200);
}

void TurtleServer::handleProgram ()
{
  const size_t bufferSize = 1024;

  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& json = jsonBuffer.parseObject(_server.arg("plain"));
  if ( ! json.success())
  {
    setCORSHeaders();
    _server.send(400, "application/json", "{\"error\": \"Invalid JSON format\"}");
    TurtleServer::_logger(400);
    return;
  }

  //json.prettyPrintTo(Serial);

  for (int i = 0; i < (int) json["CMD"].size(); i++)
  {
    String command = String((const char*) json["CMD"][i][0]);

    // Forward
    if (command == "FD")
    {
      queue.put(CMD_FORWARD, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("FORWARD"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Backward
    if (command == "BD")
    {
      queue.put(CMD_BACKWARD, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("BACKWARD"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Right
    if (command == "RT")
    {
      queue.put(CMD_RIGHT, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("RIGHT"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Left
    if (command == "LT")
    {
      queue.put(CMD_LEFT, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("LEFT"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Pen
    if (command == "PN")
    {
      queue.put(CMD_PEN, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("PEN"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Led
    if (command == "LD")
    {
      queue.put(CMD_LED_R,    (uint16_t) json["CMD"][i][1][1]);
      queue.put(CMD_LED_G,    (uint16_t) json["CMD"][i][1][2]);
      queue.put(CMD_LED_B,    (uint16_t) json["CMD"][i][1][3]);
      queue.put(CMD_LED_SHOW, (uint16_t) json["CMD"][i][1][0]); // led number

      String args = "[" +
        String((int) json["CMD"][i][1][0]) + ", " +
        String((int) json["CMD"][i][1][1]) + ", " +
        String((int) json["CMD"][i][1][2]) + ", " +
        String((int) json["CMD"][i][1][3]) +
      "]";
      TurtleServer::debug(String("LED"), args);

      continue;
    }

    // Tone
    if (command == "TE")
    {
      queue.put(CMD_TONE, (uint16_t) json["CMD"][i][1][0]);

      if ((uint16_t) json["CMD"][i][1][1] > 0)
        queue.put(CMD_SLEEP, (uint16_t) json["CMD"][i][1][1]);

      String args = "[" +
        String((int) json["CMD"][i][1][0]) + ", " +
        String((int) json["CMD"][i][1][1]) +
      "]";
      TurtleServer::debug(String("TONE"), args);

      continue;
    }

    // Sleep
    if (command == "SP")
    {
      queue.put(CMD_SLEEP, (uint16_t) json["CMD"][i][1]);
      TurtleServer::debug(String("SLEEP"), String((int) json["CMD"][i][1]));
      continue;
    }

    // Queue
    if (command == "QE")
    {
      String arg = String((const char*) json["CMD"][i][1]);

      if      (arg == "lock"  ) queue.lock();
      else if (arg == "unlock") queue.unlock();
      else if (arg == "clear" ) queue.clear();

      TurtleServer::debug(String("QUEUE"), arg);
      continue;
    }

  }

  setCORSHeaders();
  _server.send(201, "application/json", "{\"message\": \"Program added\"}");
  TurtleServer::_logger(201);
}

void TurtleServer::handleStatus ()
{
  String json = "{"
    "\"debug\": "  + String(_debug ? "true": "false") + ", "
    "\"free_memory\": "  + String(ESP.getFreeHeap(), DEC) + ", "
    "\"queue\": {"
      "\"lock\": " + String(queue.isLock() ? "true": "false") + ", "
      "\"size\": " + String(queue.maxSize()) + ", "
      "\"left\": " + String(queue.maxSize() - queue.size()) + ""
    "}"
  "}";

  setCORSHeaders();
  _server.send(200, "application/json", json);
  TurtleServer::_logger(200);
}

void TurtleServer::handleNotFound ()
{
  String method = TurtleServer::method();
  String uri = _server.uri();

  String args = "";
  for (uint8_t i = 0; i < _server.args(); i++)
  {
    String sep = (i < _server.args() - 1) ? ", " : "";
    args += "\"" + _server.argName(i) + "\": \"" + _server.arg(i) + "\"" + sep;
  }

  String json = "{"
    "\"error\": \"Not Found\", "
    "\"method\": \"" + method + "\", "
    "\"uri\": \""    + uri    + "\", "
    "\"args\": {"    + args   + "}"
  "}";

  _server.send(404, "application/json", json);
  TurtleServer::_logger(404);
}

void TurtleServer::handleDebug ()
{
  _debug = ! _debug;

  logger("server", "DEBUG MODE", (_debug ? "ON" : "OFF"));

  String json = "{\"debug\": " + String(_debug ? "true" : "false") + "}";

  _server.send(201, "application/json", json);
  TurtleServer::_logger(201);
}

void TurtleServer::handleWiFiReset ()
{
  queue.put(CMD_WIFI_RESET, (uint16_t) 1);

  String json = "{\"message\": \"Wifi settings have been reset. Please restart the robot.\"}";

  _server.send(201, "application/json", json);
  TurtleServer::_logger(201);
}

void TurtleServer::handleChangeMode ()
{
  // EEPROM.begin(512);
  // if (EEPROM.read(0)==0){
  //   EEPROM.write(0, 'f');
  // } else {
  //   EEPROM.write(0, 't');
  // }
  // Serial.print(char(EEPROM.read(0)));
  
  // if (_server.hasArg("plain")== false){ //Check if body received
  //   _server.send(200, "text/plain", "Body not received");
  //   return;
  // }
    
  // String  message = "Body received:\n";
  //         message += _server.arg("plain");
  //         message += "\n";
 
  // _server.send(200, "text/plain", message);
  // Serial.println(message);
  const size_t bufferSize = 1024;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& json = jsonBuffer.parseObject(_server.arg("plain"));
  if ( ! json.success())
  {
    setCORSHeaders();
    _server.send(400, "application/json", "{\"error\": \"Invalid JSON format\"}");
    TurtleServer::_logger(400);
    return;
  }
  String appMode = (const char*) json["app-mode"];
  if (appMode[0] != 'f' && appMode[0] != 't'){
    setCORSHeaders();
    _server.send(400, "application/json", "{\"error\": \"Invalid app-mode value\"}");
    TurtleServer::_logger(400);
    return;
  }

  EEPROM.begin(512);
  EEPROM.write(0, appMode[0]);
  EEPROM.commit();
  Serial.print("Written value:");
  Serial.println((char)EEPROM.read(0));
  
  logger("server", "APP MODE", appMode[0]);
  _server.send(201, "application/json", _server.arg("plain"));
  TurtleServer::_logger(201);
  Serial.println("Please restart the robot");
  
}