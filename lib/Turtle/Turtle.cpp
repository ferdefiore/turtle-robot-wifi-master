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
#include "Turtle.h"


Turtle::Turtle () :
_motor0(SimpleStepper::FULLSTEP,
        MOTOR0_PIN1, MOTOR0_PIN2,
        MOTOR0_PIN3, MOTOR0_PIN4),
_motor1(SimpleStepper::FULLSTEP,
        MOTOR1_PIN1, MOTOR1_PIN2,
        MOTOR1_PIN3, MOTOR1_PIN4),
_servo()
{
  //pinMode(SERVO_PIN, OUTPUT);
  _servo.attach(SERVO_PIN);
}

String Turtle::id ()
{
  return String("minirobots-") + getID();
}

String Turtle::config (const char* key)
{
  if      (strcmp(key, "pen_up")           == 0) return String(_pen_up              );
  else if (strcmp(key, "pen_down")         == 0) return String(_pen_down            );
  else if (strcmp(key, "steps_per_mm")     == 0) return String(_steps_per_mm,     10);
  else if (strcmp(key, "steps_per_degree") == 0) return String(_steps_per_degree, 10);
  else return String("");
}

void Turtle::begin ()
{
  // Set default values
  _update_firmware = false;

  _pen_up   = PEN_UP_DEFAULT_VALUE;
  _pen_down = PEN_DOWN_DEFAULT_VALUE;

  _steps_per_degree = STEPS_PER_DEGREE_DEFAULT_VALUE;
  _steps_per_mm     = STEPS_PER_MM_DEFAULT_VALUE;
}

void Turtle::connect ()
{
  String config = _connect();

  if (config.length()) _config(config);

  pen(_pen_down);
}

void Turtle::update ()
{
  logger("turtle", "Current firmware version:", VERSION);
  if (_update_firmware)
  {
    logger("turtle", "New firmware found at", _api_host.c_str());
    _update();
  }
  else
    logger("turtle", "No firmware update found at", _api_host.c_str());
}

void Turtle::handle ()
{
  if (_motor0.idle() && _motor1.idle() && !queue.empty() && !queue.isLock())
  {
    Item item = queue.get();
    //Serial.printf("Get for queue => cmd: %d, arg: %d\n", item.cmd, item.arg);

    switch (item.cmd)
    {
      case CMD_FORWARD    : forward(item.arg);    break;
      case CMD_BACKWARD   : backward(item.arg);   break;
      case CMD_RIGHT      : right(item.arg);      break;
      case CMD_LEFT       : left(item.arg);       break;
      case CMD_PEN        : pen(item.arg);        break;
      case CMD_LED_R      : led_r(item.arg);      break;
      case CMD_LED_G      : led_g(item.arg);      break;
      case CMD_LED_B      : led_b(item.arg);      break;
      case CMD_LED_SHOW   : led_show(item.arg);   break;
      case CMD_SLEEP      : sleep(item.arg);      break;
      case CMD_TONE       : _tone(item.arg);      break;
      case CMD_WIFI_RESET : wifi_reset(item.arg); break;
    }
  }

  _motor0.step();
  _motor1.step();
}

void Turtle::forward (uint16_t n)
{
  int steps = (int) n;
  steps *= _steps_per_mm;
  //Serial.printf("forward %d, %d\n", n, steps);
  _move(steps, -steps);
}

void Turtle::backward (uint16_t n)
{
  int steps = (int) n;
  steps *= _steps_per_mm;
  //Serial.printf("backward %d, %d\n", n, steps);
  _move(-steps, steps);
}

void Turtle::right (uint16_t n)
{
  int steps = (int) n;
  steps *= _steps_per_degree;
  //Serial.printf("rigth %d, %d\n", n, steps);
  _move(steps, steps);
}

void Turtle::left (uint16_t n)
{
  int steps = (int) n;
  steps *= _steps_per_degree;
  //Serial.printf("left %d, %d\n", n, steps);
  _move(-steps, -steps);
}

void Turtle::pen (uint16_t n)
{
  _servo.write( n == PEN_UP ? _pen_up : _pen_down );
  delay(100);
}

void Turtle::led_r (uint16_t n)
{
  Pixel::r = (uint8_t) n;
}

void Turtle::led_g (uint16_t n)
{
  Pixel::g = (uint8_t) n;
}

void Turtle::led_b (uint16_t n)
{
  Pixel::b = (uint8_t) n;
}

void Turtle::led_show (uint16_t pixel)
{
  if (pixel == 2)
    Pixel::showAll();
  else
    Pixel::show(pixel);
}

void Turtle::sleep (uint16_t n)
{
  delay(n);
  yield();
  noTone(BUZZER_PIN);
}

void Turtle::_tone (uint16_t n)
{
  tone(BUZZER_PIN, n);
}

/** Private methods **/

String Turtle::_connect ()
{
  String ip   = getIP();
  String mac  = getMAC();
  String ssid =  WiFi.SSID();
  String uri  = "/robot/config?ip=" + ip + "&mac=" + mac + "&version=" + VERSION + "&ssid=" + ssid;
  uint16_t port = 80;

  HTTPClient http;
  http.setTimeout(2000);

  String payload = "";

  for (uint8_t i = 0; i < API_HOSTS; i++)
  {
    _api_host = apiHosts[i];
    logger("turtle", String("Connecting to " + _api_host + "...").c_str(), false);

    http.begin(_api_host.c_str(), port, uri);
    int httpCode = http.GET();

    if (httpCode == 200)
    {
      logger("Connected!");
      payload = http.getString();
      break;
    }
    else
    {
      logger("Error:", http.errorToString(httpCode));
      continue;
    }
  }
  http.end();

  return payload;
}

void Turtle::_config (String config)
{
  const size_t bufferSize = 200;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& json = jsonBuffer.parseObject(config);
  if ( ! json.success())
  {
    logger("turtle", "Invalid json format");
    return;
  }
  //json.prettyPrintTo(Serial);

  _update_firmware = json["firmware_update"];
  if (json["pen_up"]           != -1) _pen_up           = (int)    json["pen_up"];
  if (json["pen_down"]         != -1) _pen_down         = (int)    json["pen_down"];
  if (json["steps_per_degree"] != -1) _steps_per_degree = (double) json["steps_per_degree"];
  if (json["steps_per_mm"]     != -1) _steps_per_mm     = (double) json["steps_per_mm"];

  /*
  logger("debug", "update_firmware",  String(_update_firmware).c_str());
  logger("debug", "pen_up",           String(_pen_up).c_str());
  logger("debug", "pen_down",         String(_pen_down).c_str());
  logger("debug", "steps_per_degree", String(_steps_per_degree, 10).c_str());
  logger("debug", "steps_per_mm",     String(_steps_per_mm, 10).c_str());
  //*/
}

void Turtle::_update ()
{
  String ip  = getIP();
  String mac = getMAC();
  String url = "http://" + _api_host + "/robot/firmware";
  String q   = "?ip=" + ip + "&mac=" + mac + "&version=" + VERSION;

  logger("turtle", String("Getting updates from " + _api_host + "...").c_str(), false);

  t_httpUpdate_return ret = ESPhttpUpdate.update(url + q, VERSION);

  switch (ret)
  {
    case HTTP_UPDATE_FAILED:
      logger("Failed to get updates.");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      logger("No Updates found.");
      break;
    case HTTP_UPDATE_OK:
      logger("Updated!");
      ESP.restart();
      break;
  }
}

void Turtle::_move (int n, int m)
{
  //Serial.printf("Move => 0: %d, 1: %d\n", n, m);
  _motor0.steps(n);
  _motor1.steps(m);
}

void Turtle::wifi_reset (uint16_t n)
{
  delay(1000);
  logger("turtle", "Resetting wifi credentials. Please restart the robot.");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

