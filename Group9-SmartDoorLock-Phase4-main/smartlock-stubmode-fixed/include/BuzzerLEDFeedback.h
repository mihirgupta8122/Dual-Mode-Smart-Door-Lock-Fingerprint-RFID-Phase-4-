
#pragma once
#include "Config.h"
class BuzzerLEDFeedback{ public: void begin(){ pinMode(GREEN_LED,OUTPUT); pinMode(RED_LED,OUTPUT); pinMode(BUZZER_PIN,OUTPUT); digitalWrite(GREEN_LED,LOW); digitalWrite(RED_LED,LOW); digitalWrite(BUZZER_PIN,LOW);} static void buzz(int ms=120){ digitalWrite(BUZZER_PIN,HIGH); delay(ms); digitalWrite(BUZZER_PIN,LOW);} static void green(int n=1){ for(int i=0;i<n;i++){ digitalWrite(GREEN_LED,HIGH); delay(120); digitalWrite(GREEN_LED,LOW); delay(120);} } static void red(int n=2){ for(int i=0;i<n;i++){ digitalWrite(RED_LED,HIGH); delay(120); digitalWrite(RED_LED,LOW); delay(120);} }};
