
#pragma once
#include <vector>
#include "AccessMethod.h"
#include "LockActuator.h"
#include "EventBus.h"
#include "Logger.h"
#include "Config.h"

class LockController{
  std::vector<AccessMethod*> methods; LockActuator* actuator; EventBus* bus; unsigned long lastUnlockMs=0;
public:
  LockController(std::vector<AccessMethod*> m, LockActuator* a, EventBus* b): methods(std::move(m)), actuator(a), bus(b) {}
  void begin(){ actuator->begin(); actuator->lock(); }
  void poll(){ for(auto*m: methods){ if(m && m->tryAuthenticate()){ actuator->unlock(); lastUnlockMs=millis(); if(bus) bus->publish({EventType::AccessGranted, "Auth success"}); return; }} }
  void tick(){ if(!actuator->isLocked() && AUTO_LOCK_MS>0 && (millis()-lastUnlockMs>=AUTO_LOCK_MS)){ actuator->lock(); if(bus) bus->publish({EventType::Locked, "Auto-lock"}); }}
  void forceLock(){ actuator->lock(); if(bus) bus->publish({EventType::Locked, "Forced"}); }
  void forceUnlock(){ actuator->unlock(); lastUnlockMs=millis(); if(bus) bus->publish({EventType::Unlocked, "Forced"}); }
  bool isLocked() const { return actuator->isLocked(); }
};
