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

#ifndef Turtle_h
#define Turtle_h


#include <Arduino.h>
#include <Servo.h>
#include <SimpleStepper.h>
#include <TurtleQueue.h>
#include <Pixel.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
//
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
//
#include <ArduinoJson.h>


class Turtle
{
  public:
     Turtle();

     String id();
     String config(const char*);

     void begin();
     void handle();
     void connect();
     void update();

     void forward(uint16_t);
     void backward(uint16_t);
     void right(uint16_t);
     void left(uint16_t);
     void pen(uint16_t);
     void led_r(uint16_t);
     void led_g(uint16_t);
     void led_b(uint16_t);
     void led_show(uint16_t);
     void sleep(uint16_t);
     void _tone(uint16_t);
     void wifi_reset(uint16_t);

   private:
     SimpleStepper  _motor0;
     SimpleStepper  _motor1;
     Servo          _servo;
     String         _api_host;

     bool   _update_firmware;
     int    _pen_up;
     int    _pen_down;
     double _steps_per_degree;
     double _steps_per_mm;

     String _connect();
     void   _config(String);
     void   _update();
     void   _move(int, int);
};

 #endif
