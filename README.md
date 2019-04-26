# Schwimmende Pixel

Schwimmende Pixel ist ein Projekt der Hochschule Karlsruhe. Jeder Pixel symbolisiert einen ESP32 mit einem RGB-LED-Ring der per Webseite angesteuert werden kann.
Momentan ist die Webseite für 9 Pixel also 9 ESP32 ausgelegt.
In diesem Projekt wurde sowohl die Programmierung des ESP32 als auch die Webseiten Programmierung umgesetzt.

## Getting Started
Das Projekt ist in zwei Bereiche aufgeteilt.
1. Schwimmende_Pixel_EPS
* Ansteuerung des RGB-Rings
* Ansteuerung des Oled Displays
* Websoket-anbindung zur genannten Webseite
diese sind in Schwimmende_Pixel_EPS -> src -> main.cpp zufinden
2. Webseite
* html
* js mit Websocket
* css styles

### Installing
Einfach die Github Repositories clonen bzw. herunterladen. Dann mit [VS Code](https://code.visualstudio.com/) und [Platformi](https://github.com/platformio/platformio-vscode-ide) oder mit der Arudino IDE das Projekt öffenen. Es wurden folgende dritt Biblotheken verwendet:
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) Lib für Websocket (commit 95dedf7 or version 1.2.0)
* [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) Lib für RGB-Ring (commit  8c8e38c or version 1.2.0)
* [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306) Lib für Oled (commit 5c62208 or version 1.2.9)
* [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) Lib für Grafik (commit e83e3ce or version 1.4.8)

diese müssen natürlich auch heruntergeladen werden entweder über [Platformio](https://docs.platformio.org/en/latest/librarymanager/) or in [Arduino](https://www.arduino.cc/en/guide/libraries).

### Bedienung
Über die Webseite kann man einen Rechner mit den ESPs verbinden, wichtig hierbei ist es das sich der ESP32 und die Server auf welchem die Webseite läuft, im gleichen Netzwerk befinden. Wird die Verbindung zwischen Webseite und ESP32 hergestellt, werden die LEDs weiß und symbolisieren somit den Status verbunden.
Nun können die Farben des einzelnen "Pixels" über anklicken ausgewählt werden. Momentan gibt es die klassischen Farben rot und grün, blau zur Auswahl.
Es kann vorkommen das sich die IP-Adresse des ESP32 sich ändert, diese muss dann im main.js angepasst werden.