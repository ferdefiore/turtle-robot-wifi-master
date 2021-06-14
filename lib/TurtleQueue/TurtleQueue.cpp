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


#include "TurtleQueue.h"


TurtleQueue::TurtleQueue (uint16_t length)
{
  _queueLength = length;
  _queue = (struct Item*) malloc(sizeof(struct Item) * _queueLength);
  _iPut = 0;
  _iGet = -1;
  _lock = false;
}

void TurtleQueue::put (uint8_t cmd, uint16_t arg)
{
  put((Item){cmd, arg});
}

void TurtleQueue::put (struct Item item)
{
  _queue[_iPut] = item;

  if (full())
      _iGet = _iNext(_iGet);

  else if (empty())
      _iGet = _iPut;

  _iPut = _iNext(_iPut);
}

struct Item TurtleQueue::get ()
{
  Item item = _queue[_iGet];
  _iGet = ( _iNext(_iGet) == _iPut ) ? -1 : _iNext(_iGet);
  return item;
}

boolean TurtleQueue::empty ()
{
  return _iGet == -1;
}

boolean TurtleQueue::full ()
{
  return _iPut == _iGet;
}

void TurtleQueue::lock ()
{
  _lock = true;
}

void TurtleQueue::unlock ()
{
  _lock = false;
}

void TurtleQueue::clear ()
{
  _iGet = -1;
}

boolean TurtleQueue::isLock ()
{
  return _lock;
}

int16_t TurtleQueue::maxSize ()
{
  return _queueLength;
}

int16_t TurtleQueue::size ()
{
  if (full()) return _queueLength;
  if (empty()) return 0;
  if (_iPut > _iGet) return _iPut - _iGet;
  return _queueLength - _iGet + _iPut; // _iGet > _iPut
}

/** Private methods **/

int16_t TurtleQueue::_iNext (int16_t index)
{
  return (index + 1 < _queueLength) ? index + 1 : 0;
}

int16_t TurtleQueue::_iPrev (int16_t index)
{
  return (index > 0) ? index - 1 : _queueLength - 1;
}

TurtleQueue queue(QUEUE_LENGTH);
