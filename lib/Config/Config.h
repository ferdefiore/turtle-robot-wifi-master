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

#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino


// Firmware
#define VERSION "0.3.6"

// API hosts
#define API_HOSTS 2
const String apiHosts[API_HOSTS] = {"minirobots.local", "api.minirobots.com.ar"};

// ESP8266 Pins
//
// ESP8266 Pin functions
// https://www.esp8266.com/wiki/lib/exe/detail.php?id=esp8266_gpio_pin_allocations&media=pin_functions.png
//
#define MOTOR0_PIN1  D0
#define MOTOR0_PIN2  D1
#define MOTOR0_PIN3  D2
#define MOTOR0_PIN4  D3
//
#define MOTOR1_PIN1  D4
#define MOTOR1_PIN2  D5
#define MOTOR1_PIN3  D6
#define MOTOR1_PIN4  D7
//
#define BUZZER_PIN   D8
#define SERVO_PIN    10

// Commands
#define CMD_FORWARD    B0001
#define CMD_BACKWARD   B0010
#define CMD_RIGHT      B0011
#define CMD_LEFT       B0100
#define CMD_PEN        B0101
#define CMD_LED_R      B0110
#define CMD_LED_G      B0111
#define CMD_LED_B      B1000
#define CMD_LED_SHOW   B1001
#define CMD_SLEEP      B1010
#define CMD_TONE       B1011
#define CMD_WIFI_RESET B1111

// Pen values
#define PEN_UP                 0
#define PEN_UP_DEFAULT_VALUE   0
#define PEN_DOWN               1
#define PEN_DOWN_DEFAULT_VALUE 15

// Rotation math
#define BASE_RADIUS                  45
#define BASE_CIRCUMFERENCE           (2 * PI * BASE_RADIUS)
#define WHEEL_RADIUS                 33
#define WHEEL_CIRCUMFERENCE          (2 * PI * WHEEL_RADIUS)
#define GEAR_REDUCTION_RATIO         63.68395 // See SimpleStepper.h documentation
#define STEPS_PER_REVOLUTION         32 // Full step sequence
#define STEPS_PER_OUTPUT_REVOLUTION  (GEAR_REDUCTION_RATIO * STEPS_PER_REVOLUTION)
// Default values
#define STEPS_PER_DEGREE_DEFAULT_VALUE  ((BASE_CIRCUMFERENCE / 360) * STEPS_PER_OUTPUT_REVOLUTION / WHEEL_CIRCUMFERENCE)
#define STEPS_PER_MM_DEFAULT_VALUE      (STEPS_PER_OUTPUT_REVOLUTION / WHEEL_CIRCUMFERENCE)


// Function prototypes
void logger(const char*, bool nl=true);
void logger(const char*, const char*, bool nl=true);
void logger(const char*, const char*, const char*, bool nl=true);
String getIP();
String getMAC();
String getID();
