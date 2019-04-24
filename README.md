# Schwimmende Pixel

Schwimmende Pixel ist ein Projekt der Hochschule Karlsruhe. Jeder Pixel symbolisiert einen ESP32 mit einem RGB-LED-Ring der per Webseite angesteuert werden kann.
Momentan ist die Webseite für 9 Pixel also 9 ESP32 ausgelegt.
In diesem Projekt wurde sowohl die Programmierung des ESP32 als auch die Webseiten Programmierung umgesetzt.

## Getting Started
Das Projekt ist in zwei Bereiche aufgeteilt.
1. Schwimmende_Pixel_EPS
Dazu gehört:
* Ansteuerung des RGB-Rings
* Ansteuerung des Oled Displays
* Websoket anbindung zur genannten Webseite.
2. Webseite
Dazu gehört:
* html
* js mit Websocket

### Sonstiges
Das Projekt wurde mit mit [VS Code](https://code.visualstudio.com/) und [Platformi](https://github.com/platformio/platformio-vscode-ide) umgesetzt.

### Bedienung
Über die Webseite kann man sich mit den ESPs verbinden. Wird die verbindung zwischen Webseite und ESP32 hergestellt, werden die LEDs Weiß.
Nun können die Farben des einzelnen "Pixels" ausgewählt werden, es gibt Momentan die klassischen Farben: Rot, Grün, Blau.

### Weiterentwicklung
Das Projekt könnte durch mehrere ESPs noch erweitert werden. Damit sollte es möglich sein einen Schriftzug dazustellen.
Zusätzlich könnte man die LED-Ringe zu verschiedenen Muster zusammenschließen und sie auf unterschiedlichste Informationen zb. Musik reagiern lassen.

### Fazit
Insgesamt war es ein sehr interesantes Projekt gerade durch die vielen verschiedenen mechatronischen Komponenten.
