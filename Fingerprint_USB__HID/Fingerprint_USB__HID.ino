// Fingerabdruck Passwort Eingabe über USB HID Keyboard Emulator
// fingerprint usb hid keyboard
// USB Macro Keypad 
// shortcuts key fingerprint
// https://esp8266-server.de/fingerprint.html
// Arduino Leonardo Micro + FPC1021A fingerprint ID809
// Die Makros und Texte zum Fingerabdrucken befinden sich im "Makros.h"
// Bitte vorher das Sketsh "Fingerprint_Einlesen.ino" laden und die Finger im Sensor registrieren.

#include <stdint.h>
#include <Adafruit_Fingerprint.h>
#include "KeyboardDE.h"           // Speziel für Deutsche Tastaturbelegung
#include "MakrosPass.h"           // Hier bitte die Makros und Passwörter hinterlegen

#define SensorPassword 0x00000000          // Fingerprint password, falls gesetzt, hier eintragen (4Byte)

#define TouchSensor 2             // Pin für Fingererkennung
#define active_low  1             // 0- FPC1021A, 1- R503 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1,SensorPassword);

// ---------------- Setup ------------------------------------------
void setup(void)
{
  Serial.begin(115200);         // Kommunikation mit PC Statusmeldungen / Debug Ausgaben
  finger.begin(57600);          // Kommunikation mit Singerabdrucksensor (57600baud ist standart)
  pinMode(TouchSensor, INPUT); 
  if (finger.verifyPassword())
  {
    if (Serial) Serial.println("Fingerabdrucksensor angeschlossen");
    Keyboard.begin();            // Starte USB HID Tastatur Emulation
  }
  else                           // Fingerabdrucksensor antwortet nicht
  {
    while(!Serial)               // Warte bis serielle Terminal gestartet wurde
    Serial.println("Fingerabdrucksensor antwortet nicht");
    while(1);                    // Programmausführung stoppen 
  } 
}

uint8_t id;

void loop(void) 
{ 
    int FingerID = getFingerprintIDez();             // Erkennung starten
    if ( FingerID > 0 )
    {
    KeyOutput(data[FingerID-1]);   // Wenn erkannt, Keybord ausgabe des Strings aus dem Array auf der ID - Position
    while (!(active_low ^ digitalRead(TouchSensor))); // hier Bleiben bis neue Berührung
    }
    
}

int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_PURPLE); // Fingerabdruck eingelesen
  p = finger.image2Tz();
  finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) 
  {
    finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE); // Fingerabdruck nicht im Datenbank
    finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 500, FINGERPRINT_LED_RED);
    return -1;
  }
  
  finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_PURPLE);  // Fingerabdruck im Datenbank gefunden
  finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 200, FINGERPRINT_LED_BLUE);
  return finger.fingerID;
}


// ---------------- HEX String in Integer umwandeln  -----------------------

unsigned int hexToDec(String hexString)
{

  unsigned int decValue = 0;
  int nextInt;
  hexString.toUpperCase();
  for (int i = 0; i < hexString.length(); i++)
  {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}


// ---------- Ausgabe des Textes über HID-Keybord und Ausführung der Befehle -----

void KeyOutput(const String str)
{
  int pos = str.indexOf('§');     
  while (pos >= 0)
  {
    if (pos > 0) Keyboard.print(str.substring(0, pos - 1));
    str.remove(0, pos + 1);
    char kode = str.charAt(0);
    pos = str.indexOf(' ');
    String Daten = str.substring(1, pos);
    byte z;
    switch (kode)             // Befehle
    {
      case 'w':
        z = hexToDec(Daten);
        Keyboard.write(z);
        break;
      case 't':
        delay(Daten.toInt());
        break;
      case 'p':
        z = hexToDec(Daten);
        Keyboard.press(z);
        break;
      case 'r':
        z = hexToDec(Daten);
        Keyboard.release(z);
        break;
      case 'a':
        Keyboard.releaseAll();
        break;
    }
    str.remove(0, pos + 1);
    pos = str.indexOf('§');
  }
  if (str.length() > 0)Keyboard.print(str);
}
