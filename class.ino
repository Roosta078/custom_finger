//#include <Adafruit_Fingerprint.h>
#include "custom_finger.h"

HardwareSerial mySerial(2);
custom_finger finger = custom_finger(&mySerial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (finger.fingerprint_init()){
    Serial.println("Found Sensor");
  } else {
    Serial.println("Could not find Sensor");
    while(1);;
  }

  finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_BLUE);
  
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void enroll_handler(){
  uint8_t id;
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    Serial.println("Invalid ID");
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  if(finger.enroll(id) == FINGERPRINT_OK){
    Serial.println("Enroll successful");
  }else{
    Serial.println("Error enrolling");
  }
}

void verify_handler(){
  Serial.println("Ready to verify a fingerprint!");
  if (finger.verify() == FINGERPRINT_OK){
    Serial.println("Access granted");
  }else{
    Serial.println("Access denied");
  }
}

void delete_handler(){
  uint8_t id;
  Serial.println("Ready to delete a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    Serial.println("Invalid ID");
     return;
  }
  Serial.print("Deleting ID #");
  Serial.println(id);

  if(finger.remove(id) == FINGERPRINT_OK){
    Serial.println("Delete successful");
  }else{
    Serial.println("Error deleting");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t sel;
  Serial.println("Select mode:");
  Serial.println("1) Enroll");
  Serial.println("2) Verify");
  Serial.println("3) Delete");
  sel = readnumber();
  switch (sel){
    case 1:
      enroll_handler();
      break;
     case 2:
      verify_handler();
      break;
     case 3:
      delete_handler();
      break;
    default:
      Serial.println("Command not recognized");
  }
}
