
#pragma once
#include "LockActuator.h"
#include "Config.h"
class SolenoidLockActuator: public LockActuator{
  bool locked=true;
public:
  void begin() override { pinMode(RELAY_PIN,OUTPUT); digitalWrite(RELAY_PIN,LOW); }
  void lock()  override { digitalWrite(RELAY_PIN,LOW);  locked=true; }
  void unlock()override { digitalWrite(RELAY_PIN,HIGH); locked=false; }
  bool isLocked() const override { return locked; }
};
