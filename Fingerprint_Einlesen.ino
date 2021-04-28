// Fingerabdruck Passwort Eingabe über USB HID Keyboard Emulator
// fingerprint usb hid keyboard
// Resistiven Touchscreen Panel als USB Macro Keypad 
// shortcuts programmierbare Keyboard
// Frei Programmierbare USB Makro Tastatur
// Arduino Leonardo Micro + Touchscreen Panel

#include <stdint.h>
#include <Adafruit_Fingerprint.h>

#define mySerial Serial1
#define TouchSensor 2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



// ---------------- Setup ------------------------------------------

void setup(void)
{
  Serial.begin(115200);
  finger.begin(57600);
  while(!Serial); 
  Serial.println("Start!");
  pinMode(TouchSensor, INPUT); 
  pinMode(TouchSensor, INPUT); 
  if (finger.verifyPassword() and Serial)Serial.println("Fingerabdrucksensor angeschlossen");
}

uint8_t id;

void loop(void) 
{ 
while (digitalRead(TouchSensor))
  {
    int FingerID = getFingerprintIDez();
    if (FingerID<0) Serial.println("Nicht gefunden");
    else
      {
          Serial.print("Gefunden ID #");
          Serial.println(FingerID);
      }

  }  
  
if (Serial.available())     //Wenn Eingabe erfolgt
  {
    char  Wahl = Serial.read();     //Hole eingegebenes Zeichen
    Serial.flush();
    Serial.println(Wahl);     //Zeige eingegebenes Zeichen
    switch (Wahl)             //Auswahlsteuerung
    {
      case 'r':
        Serial.println("Bitte ID eingeben");
        id = readnumber();
          if (id == 0) 
          {// ID #0 not allowed, try again!
              break;
          }
          Serial.print("Speicher ID #");
          Serial.println(id);
          while (!  getFingerprintEnroll() )  delay(100);
        break;
      case 'l': // Löschen
      Serial.println("Bitte gebe die ID zum Löschen");
        id = readnumber();
          if (id == 0) 
          {// ID #0 not allowed, try again!
              break;
          }
          Serial.print("ID #");
          Serial.print(id);
          Serial.println(" Löschen? j/n");
          Serial.flush();
          while (!Serial.available());
          Wahl = Serial.read();
          Serial.println(Wahl);
          if ( Wahl == 'j') deleteFingerprint(id);
        break;
        case 't':
        for (int finger = 1; finger < 10; finger++) {
    downloadFingerprintTemplate(finger);
  }
        break;
    } // Auswahl
  }   // Wenn Eingabe erfolgt
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Gelöscht!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Falsche ID");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
  //    Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Abbildung umgewandelt");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Abbildung undeutlich");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Fingerabdruck entfernen");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Bitte den gleichen Finger auflegen");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Abbildung erhalten");
      break;
    case FINGERPRINT_NOFINGER:
 //     Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK)  return -1;

  return finger.fingerID;
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t downloadFingerprintTemplate(uint16_t id)
{
  Serial.println("------------------------------------");
  Serial.print("Attempting to load #"); Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // OK success!

  Serial.print("Attempting to get #"); Serial.println(id);
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
      break;
   default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 2136 && (millis() - starttime) < 5000) 
  {
 
      if (mySerial.available()) 
      {
         // printHex(mySerial.read(), 2);
         uint8_t k = mySerial.read();
          i++;   
      }
}
  Serial.println();
  Serial.println(millis()- starttime);
}



void printHex(int num, int precision) {
    char tmp[16];
    char format[128];

    sprintf(format, "%%.%dX", precision);

    sprintf(tmp, format, num);
    Serial.print(tmp);
}
