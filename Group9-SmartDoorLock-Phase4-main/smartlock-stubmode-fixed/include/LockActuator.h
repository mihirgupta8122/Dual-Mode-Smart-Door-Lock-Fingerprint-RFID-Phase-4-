
#pragma once
#include <Arduino.h>
class LockActuator{ public: virtual ~LockActuator()=default; virtual void begin()=0; virtual void lock()=0; virtual void unlock()=0; virtual bool isLocked() const =0; };
