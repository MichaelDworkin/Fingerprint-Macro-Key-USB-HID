/* 
Programm zum einlesen der Fingerabdrucke
Fingerabdruck Registrieren              "r"
Fingerabdruck Löschen                   "l"
Passwort für Fingerabdrucksensor setzen "p" 
*/

#include <stdint.h>
#include <Adafruit_Fingerprint.h>

#define TouchSensor 2
#define active_low  1                   // 0- FPC1021A, 1- R503 

#define FingerprintPassword 0x00000000     // Aktuelle Sensor Passwort
#define FingerprintPasswordNeu 0x00000000  // Neue Sensor Passwort wird mit Befehl "p" gesetzt 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1,FingerprintPassword);

// ---------------- Setup ------------------------------------------

void setup(void)
{
  Serial.begin(115200);
  finger.begin(57600);                       // Standarteinstellung 57600 baud
  while(!Serial);                            // Hier warten bis serielle Terminal gestartet wurde
  Serial.println("Start!");                  
  pinMode(TouchSensor, INPUT); 
  pinMode(TouchSensor, INPUT); 
  if (finger.verifyPassword() and Serial)Serial.println("Sensor Passwort stimmt überein");
}

uint8_t id;
int FingerID = -1;

void loop(void) 
{ 
  FingerID = getFingerprintIDez();  
  if (FingerID>=0) 
  {
     Serial.print("Gefunden ID #");
     Serial.println(FingerID);
     delay(10);
  }            // Ende Fingerabdruck erkannt    
  
  if (Serial.available())           //Wenn Eingabe erfolgt
  {
     char  Wahl = Serial.read();    //Hole eingegebenes Zeichen
     Serial.flush();
     Serial.println(Wahl);          //Zeige eingegebenes Zeichen
     switch (Wahl)                  //Auswahlsteuerung
     {
            case 'r':               //Fingerabdruck aufnehmen
            Serial.println("Bitte ID eingeben");
            id = readnumber();
            Serial.print("Speicher ID #");
            Serial.println(id);
            while (!  getFingerprintEnroll() )  delay(100);
            break;
            
            case 'l':           // Löschen
            Serial.println("Bitte gebe die ID zum Löschen");
            id = readnumber();
            Serial.print("ID #");
            Serial.print(id);
            Serial.println(" Löschen? j/n");
            Serial.flush();
            while (!Serial.available());
            Wahl = Serial.read();
            Serial.println(Wahl);
            if ( Wahl == 'j') deleteFingerprint(id);
            break;
            
            case 'p': // Password setzen
            Serial.print("Setze Passwort...");
            uint8_t p = finger.setPassword(FingerprintPasswordNeu);
            if (p == FINGERPRINT_OK) 
            {
                  Serial.println("OK Passwort lautet jetzt: "); 
                  Serial.println(FingerprintPasswordNeu, HEX); 
                  Serial.println("Vergesse nicht die Variable \"FingerprintPassword\" im Sketh anzupassen!");
            } 
            else 
            {
                  Serial.println("Fehler"); 
            }
            break;
         }    // Ende switch
  }   // Ende serielle Eingabe 
}     // Ende loop Schleife

uint8_t deleteFingerprint(uint8_t id) 
{
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
uint8_t getFingerprintEnroll() 
{
  int p = -1;
  Serial.println("Bitte den Finger auf das Sensor auflegen");
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
           case FINGERPRINT_OK:
           Serial.println("Abbild ehalten");
           break;
           case FINGERPRINT_NOFINGER:
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
    }       // Ende switch
  }         // Ende Schleife bis Finger eingelesen wird

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
  Serial.println("Finger vom Sensor entfernen");
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  delay(1000);
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
      Serial.println("Abbildung umgewandelt");
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
  Serial.print("Erzeuge Modell für #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerabdruck Model erstellt!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerabdrücke stimmten nicht überein");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Gespeichert!");
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
 // if (p == FINGERPRINT_NOFINGER) Serial.println("No finger detected");
  if (p != FINGERPRINT_OK)  return -1;
  finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 200, FINGERPRINT_LED_PURPLE);
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) 
  {
    finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);
    finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 500, FINGERPRINT_LED_RED);
    return -1;
  }
  
  finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);
  finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 500, FINGERPRINT_LED_BLUE);
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
