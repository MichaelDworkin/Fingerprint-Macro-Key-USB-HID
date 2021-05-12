# Automatische Passwort Eingabe mit Fingerabdrucksensor
ATmega32U4 (Arduino Pro Micro) Emuliert eine USB HID Tastatur und führt Makros aus. So lassen sich beliebige Texte und Tastenkombinationen an den Rechner senden.

Für den Rechner sieht es so aus, als würde die Eingabe über USB Keyboard erfolgen.
Pro Fingerabdruck lässt sich eine Macro belegen. Nach dem berühren des fingerprint sensors und erkennung des Fingerabdrucks wird dazugehörige Macro ausgeführt.
Es gibt auch die Möglichkeit die Zeitverzögerungen im Macro zu hinterlegen. Auf diese Weise kann die Reaktionszeit des Computers, z. B. bei öffnen der Programme, berücksichtigt werden.
Diese Passwort Manager ist, durch Emulation einer USB Tastatur auf der Hardwareebene, Betriebssystem unabhängig. Kein eingriff in die Software nötig. Und kann mit Systemen verwendet werden, die durch Administrator voll gesperrt sind. Nur freie USB Port ist erforderlich. 
[Projekt Webseite](https://esp8266-server.de/fingerprint.html)

![Schaltplan / Anschlussplan R503 Fingerprint und Arduino Pro Micro(https://github.com/MichaelDworkin/Fingerprint-Macro-Key-USB-HID/blob/main/IMG_20210428_085550.jpg?raw=true)


# Fingerprint Passwort Manager (USB Keyboard Emulation)
Fingerprint USB keyboard emulation for password automatic input. Arduino Leonardo (ATmega32U4) and FPC1021A Fingerprint
![Fingerprint password manager USB keyboard emulation](https://github.com/MichaelDworkin/Fingerprint-Macro-Key-USB-HID/blob/main/IMG_20210428_085550.jpg?raw=true)
