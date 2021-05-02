// Fingerabdruck Passwort Eingabe über USB HID Keyboard Emulator
// fingerprint usb hid keyboard
// USB Macro Keypad 
// shortcuts key fingerprint
// https://esp8266-server.de/fingerprint.html
// Arduino Leonardo Micro + FPC1021A fingerprint ID809

// Befehlformat: "§", Befehl, Wert, Leerzeichen (Beispiel "Wort1§t1000 Wort2" Befehl warte 1 sekunde)
// §p - Press         "§p0x80 "  Strg drücken
// §r - Release       "§r0x80 "  Strg loslassen
// §a - Release All   "§a "      Alle Tasten Loslassen
// §w - Write         "§w0xB0 "  Enter klicken
// §t - Timer         "§t3000  " 3 Sekunden abwarten
// Scancodes Zuordnung bitte hier entnehmen
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h
// Die Makros und Texte zum Fingerabdrucken befinden sich im "Makros.h"

#include <stdint.h>
#include <Adafruit_Fingerprint.h>
#include "KeyboardDE.h"
#include "MakrosPass.h"
#define SensorPassword 0

#define mySerial Serial1
#define TouchSensor 2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial,SensorPassword);

// ---------------- Setup ------------------------------------------

void setup(void)
{
  Serial.begin(115200);
  finger.begin(57600);
  pinMode(TouchSensor, INPUT); 
  pinMode(TouchSensor, INPUT); 
  if (finger.verifyPassword() and Serial)Serial.println("Fingerabdrucksensor angeschlossen");
  Keyboard.begin();
}

uint8_t id;

void loop(void) 
{ 
while (digitalRead(TouchSensor))
  {
    int FingerID = getFingerprintIDez();
    if (FingerID>0)KeyOutput(data[FingerID-1]); 
  }  
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
