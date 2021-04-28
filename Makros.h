// Fingerabdruck Passwort Eingabe über USB HID Keyboard Emulator

// Befehlformat: "§", Befehl, Wert, Leerzeichen (Beispiel "Wort1§t1000 Wort2" Befehl warte 1 sekunde)
// §p - Press         "§p0x80 "  Strg drücken
// §r - Release       "§r0x80 "  Strg loslassen
// §a - Release All   "§a "      Alle Tasten Loslassen
// §w - Write         "§w0xB0 "  Enter klicken
// §t - Timer         "§t3000  " 3 Sekunden abwarten
// Scancodes Zuordnung bitte hier entnehmen
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h

#define Datensaetze 10
const String data[Datensaetze] =
{
"Finger 1§w0xB0 ", 
"Finger 2§w0xB0 ",
"Finger 3§w0xB0 ",
"Finger 4§w0xB0 ",
"Finger 5§w0xB0 ",
"Finger 6§w0xB0 ",
"Finger 7§w0xB0 ",
"Finger 8§w0xB0 ",
"Finger 9§w0xB0 ",
"Finger 10§w0xB0 "
};