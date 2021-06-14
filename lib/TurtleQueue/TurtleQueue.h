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


#ifndef TurtleQueue_h
#define TurtleQueue_h


#include <Arduino.h>


#define QUEUE_LENGTH 1024


struct Item
{
  uint8_t  cmd;
  uint16_t arg;
};

class TurtleQueue
{
  public:
    TurtleQueue(uint16_t length);

    void put(struct Item);
    void put(uint8_t cmd, uint16_t arg);

    void lock();
    void unlock();
    void clear();

    struct Item get();

    boolean empty();
    boolean full();
    boolean isLock();

    int16_t maxSize();
    int16_t size();

  private:
    boolean _lock;

    struct Item *_queue;

    uint16_t _queueLength;

    int16_t _iPut;
    int16_t _iGet;
    int16_t _iNext(int16_t);
    int16_t _iPrev(int16_t);
};

extern TurtleQueue queue;

#endif
