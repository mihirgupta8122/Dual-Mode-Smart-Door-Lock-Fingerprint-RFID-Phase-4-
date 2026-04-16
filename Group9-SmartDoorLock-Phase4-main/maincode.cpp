// C++ code
 
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <vector>
#include <algorithm>
#include <Adafruit_Fingerprint.h>
 
 
int changeIFR_ID = 0;
 
int doorStatus = 0;
 
int pos = 0;
int target_pos = 0;
 
int timer = 0;
unsigned long openTime = millis();
const long timeout = 15000;   // 15 seconds in milliseconds
 
Servo servo_9;
 
int speakerPin = 15;
 
//Vector group of known IFR IDs
std::vector<String> known_IFR_IDS;
 
//Setup for MFC522 scanner
#define SS_PIN  5
#define RST_PIN 21
MFRC522 rfid(SS_PIN, RST_PIN);
 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
int nextID = 1;//Next ID to store fingerprints
 
//Fingerprint state management varibles
enum EnrollState {
  IDLE,
  WAIT_FINGER_1,
  CONVERT_1,
  REMOVE_FINGER,
  WAIT_FINGER_2,
  CONVERT_2,
  CREATE_MODEL,
  STORE_MODEL,
  DONE
};
 
EnrollState enrollState = IDLE;
unsigned long enrollTimer = 0;
 
//This function is run to get the app ready
void setup()
{
  //Revice data from port 2
  pinMode(2, INPUT_PULLUP);
 
  //To check if need to add a need IFR code
  pinMode(4, INPUT_PULLUP);
 
  //Pin 25 for opening
  pinMode(25, OUTPUT);
  //Pin 33 for closing
  pinMode(33, OUTPUT);
 
  //For buzzer
  pinMode(speakerPin, OUTPUT);
  //Start quiet
  noTone(speakerPin);
 
  //Initial server
  servo_9.attach(26, 500, 2500);
  //Make sure the servo position is set at 0 initialy
  servo_9.write(0);
 
  //This code is for initializing the MFC522 scanner
  Serial.begin(115200);
  //Make sure it uses the default pins
  SPI.begin(18, 19, 23, 5); // SCK, MISO, MOSI, SS
  delay(50);
  rfid.PCD_Init();
  Serial.println("MFRC522 Ready");
 
  //Initialize the Fingerprint scanner
  Serial2.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);
 
  //COnfirm if fingerprint scanner properly wired and connected.
  delay(1000);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    finger.emptyDatabase();
  } else {
    Serial.println("Did not find fingerprint sensor :(");
  }
 
  //Log fingerprint scanner info
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);
}
 
//This is the main app loop
void loop()
{
  //Get input, check if we want to add a new RFID or fingerprint
  changeIFR_ID = digitalRead(4);
  if(changeIFR_ID == LOW){
    Serial.println("Starting enrollment...");
    enrollState = WAIT_FINGER_1;
    digitalWrite(25, HIGH);
    digitalWrite(33, HIGH);
    tone(speakerPin, 300);
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
      //Ardunio is trying to enroll a fingerprint here
      handleFingerprintEnroll();
      if(enrollState == IDLE){
        break;
      }
 
      // The Arduino is "waiting" here for an IFR ID to be scanned
      if (!rfid.PICC_IsNewCardPresent()) {
        //return;
      }else if (!rfid.PICC_ReadCardSerial()) {
        //return;
      }else{
        String IFR_ID = "";
        for (byte i = 0; i < rfid.uid.size; i++) {
          // Add the separator and leading zero if needed
          IFR_ID += (rfid.uid.uidByte[i] < 0x10 ? ":0" : ":");
          // Convert the byte to a Hex String explicitly
          IFR_ID += String(rfid.uid.uidByte[i], HEX);
        }
        IFR_ID.toUpperCase();
        //Print the IFR ID
        Serial.println(IFR_ID);
        Serial.println("New ID added");
        known_IFR_IDS.push_back(IFR_ID);
 
        rfid.PICC_HaltA();
        break;
      }
    }
    enrollState = IDLE;
    noTone(speakerPin);
    digitalWrite(25, LOW);
    digitalWrite(33, LOW);
  }
 
  if (!rfid.PICC_IsNewCardPresent()) {
    //return;
  }else if (!rfid.PICC_ReadCardSerial()) {
    //return;
  }else{
    //Get the IFR ID
    String IFR_ID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      // Add the separator and leading zero if needed
      IFR_ID += (rfid.uid.uidByte[i] < 0x10 ? ":0" : ":");
      // Convert the byte to a Hex String explicitly
      IFR_ID += String(rfid.uid.uidByte[i], HEX);
    }
    IFR_ID.toUpperCase();
    //Print the IFR ID
    Serial.println(IFR_ID);
 
    rfid.PICC_HaltA();
 
    //If the card reader get an id that matches
    auto it = std::find(known_IFR_IDS.begin(), known_IFR_IDS.end(), IFR_ID);
 
    if (it != known_IFR_IDS.end()) {
      Serial.println("Known user" + IFR_ID);
      target_pos = 180;
      timer = 0;
      tone(speakerPin, 300);
      digitalWrite(25, HIGH);
      digitalWrite(33, LOW);
      for (; pos <= target_pos; pos++) {
        servo_9.write(pos);
        delay(15);
      }
      openTime = millis();
      noTone(speakerPin);
      digitalWrite(25, LOW);
    } else {
      Serial.println("Unknown user: " + IFR_ID);
    }
  }
  int fingerPrintScan = getFingerprintIDez();
  if(fingerPrintScan >= 0){
    Serial.println("Known user" + fingerPrintScan);
    target_pos = 180;
    timer = 0;
    tone(speakerPin, 300);
    digitalWrite(25, HIGH);
    digitalWrite(33, LOW);
    for (; pos <= target_pos; pos++) {
      servo_9.write(pos);
      delay(15);
    }
    noTone(speakerPin);
    digitalWrite(25, LOW);
    openTime = millis();
  }
 
 
  //Begin closing the door once count down begins
  if (millis() - openTime < 15000) {
    digitalWrite(33, LOW);
  }else{
    digitalWrite(25, LOW);
    target_pos = 0;
    if(pos > target_pos){
      digitalWrite(33, HIGH);
      pos--;
      servo_9.write(pos);
      tone(speakerPin, 200);
    }else{
      //Already closed
      digitalWrite(33, LOW);
    }
    if(pos == target_pos){
      noTone(speakerPin);
    }
  }
  return;
}
 
 
//This function handle enrolling a fingerprint
void handleFingerprintEnroll() {
  int p;
 
  switch (enrollState) {
 
    case IDLE:
      return; // do nothing
 
    case WAIT_FINGER_1:
      p = finger.getImage();
      if (p == FINGERPRINT_OK) {
        Serial.println("Image 1 taken");
        enrollState = CONVERT_1;
      }
      break;
 
    case CONVERT_1:
      if (finger.image2Tz(1) == FINGERPRINT_OK) {
        Serial.println("Image 1 converted");
        Serial.println("Remove finger");
        enrollState = REMOVE_FINGER;
      } else {
        enrollState = IDLE;
      }
      break;
 
    case REMOVE_FINGER:
      if (finger.getImage() == FINGERPRINT_NOFINGER) {
        Serial.println("Place same finger again");
        enrollState = WAIT_FINGER_2;
      }
      break;
 
    case WAIT_FINGER_2:
      p = finger.getImage();
      if (p == FINGERPRINT_OK) {
        Serial.println("Image 2 taken");
        enrollState = CONVERT_2;
      }
      break;
 
    case CONVERT_2:
      if (finger.image2Tz(2) == FINGERPRINT_OK) {
        Serial.println("Image 2 converted");
        enrollState = CREATE_MODEL;
      } else {
        enrollState = IDLE;
      }
      break;
 
    case CREATE_MODEL:
      if (finger.createModel() == FINGERPRINT_OK) {
        Serial.println("Model created");
        enrollState = STORE_MODEL;
      } else {
        enrollState = IDLE;
      }
      break;
 
    case STORE_MODEL:
      if (finger.storeModel(nextID) == FINGERPRINT_OK) {
        nextID++;
        Serial.println("Stored!");
      }
      enrollState = DONE;
      break;
 
    case DONE:
      Serial.println("Enrollment complete");
      enrollState = IDLE;
      break;
  }
}
 
//Test if fingerprint matches
// returns -1 if failed, otherwise returns a positive ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;
 
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;
 
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;
 
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}
 