
#pragma once
#include "AccessMethod.h"
#include "Config.h"
#include "Logger.h"
#include "AuthStorage.h"

#ifdef STUB_MODE
class RFIDAccess: public AccessMethod{
  AuthStore* store; String lastUid;
public:
  RFIDAccess(AuthStore* s): store(s) {}
  void begin(){ Logger::info("RFID (STUB) ready — press 'r' to simulate auth, 'a' for admin"); }
  String getLastUid() const { return lastUid; }
  bool tryAuthenticate() override{ if(Serial.available()){ int c=Serial.read(); if(c=='r'){ lastUid = "STUB UID"; Logger::info("(STUB) RFID auth success"); return true; } } return false; }
  bool checkAdminTap(){ if(Serial.available()){ int c=Serial.peek(); if(c=='a'){ Serial.read(); Logger::info("(STUB) Admin tap"); return true; } } return false; }
  bool readOneUID(String &uidOut){ if(Serial.available()){ int c=Serial.read(); if(c=='c'){ uidOut = "DE AD BE EF"; return true; } if(c=='x'){ uidOut = "DE AD BE EF"; return true; } } return false; }
};
#else
#include <SPI.h>
#include <MFRC522.h>
class RFIDAccess: public AccessMethod{
  MFRC522 mfrc; AuthStore* store; String lastUid;
public:
  RFIDAccess(AuthStore* s): mfrc(MFRC522(MFRC522_SS_PIN, MFRC522_RST_PIN)), store(s) {}
  void begin(){ SPI.begin(); mfrc.PCD_Init(); Logger::info("RC522 ready"); }
  String getLastUid() const { return lastUid; }
  static String uidToString(MFRC522::Uid* uid){ String out=""; for(byte i=0;i<uid->size;i++){ if(uid->uidByte[i]<0x10) out+="0"; out+=String(uid->uidByte[i],HEX); if(i+1<uid->size) out+=" "; } out.toUpperCase(); return out; }
  bool tryAuthenticate() override{ if(!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) return false; lastUid = uidToString(&mfrc.uid); bool ok = containsUID(*store, lastUid); mfrc.PICC_HaltA(); mfrc.PCD_StopCrypto1(); if(ok) Logger::info(String("RFID authorized: ")+lastUid); return ok; }
  bool checkAdminTap(){ if(!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) return false; String uid=uidToString(&mfrc.uid); mfrc.PICC_HaltA(); mfrc.PCD_StopCrypto1(); if(uid==ADMIN_UID){ Logger::info("Admin card detected"); return true;} return false; }
  bool readOneUID(String &uidOut){ if(!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) return false; uidOut=uidToString(&mfrc.uid); mfrc.PICC_HaltA(); mfrc.PCD_StopCrypto1(); return true; }
};
#endif
