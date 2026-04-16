
#pragma once
#include <Arduino.h>
#include <functional>
#include <vector>

enum class EventType{ AccessGranted, AccessDenied, Locked, Unlocked, SystemFault };
struct Event{ EventType type; String message; };
class EventBus{ using Listener=std::function<void(const Event&)>; std::vector<Listener> ls; public: void subscribe(Listener l){ ls.push_back(l);} void publish(const Event&e){ for(auto &f: ls) f(e);} };
