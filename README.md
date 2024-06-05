# IoT_Edge_SW
IoT-Edge-SW Entwickler Bewerber Chalange

## Beschreibung 
Die Applikation simmuliert das Senden einer Nachricht von einem IoT Device an einen Azure IoTHub. Zu diesem Zweck wird eine Json Nachricht aus einem File geladen 
und eine entsprechende Nachricht generiert. Über einen Client wird diese Nachricht via MQTT an einen Azure IoTHub gesendet.

Des weiteren soll eine "Direct Methode" am IoTHub eingerichtet werden, der eine Nachricht an den Client sendet. Ein entsprechder Callback ist am Client implementiert.

## Abhängigkeiten
Für die Erzeugung der Applikation ist der Download der folgenden Libraries notwendig (Folgen Sie den Instruktionen auf den entsprechenden Github Seiten):

* rapidjson
  git clone https://github.com/Tencent/rapidjson
  Header only Library - daher ist der Clone ausreichend

* azure-iot-sdk-c
  * git clone https://github.com/Azure/azure-iot-sdk-c.git
  * cd azure-iot-sdk-c
  * git checkout LTS_07_2022_Ref01
  * git submodule update --init --recursive
  * mkdir cmake
  * cd cmake
  * make ..
  * cmake --build .

## Download des Projekt und im Visual Studio 2022 anzeigen
Klonen des Projekts mit 'cmd' oder 'Git Bash' mit 
git clone https://github.com/jleixneri/IoT_Edge_SW.git

Öffnen des Visual Studios und 'Öffnen eines lokalen Ordners' anklicken. Ordner in dem der Klone geladen wurde öffnen. Default Config ist x64 Debug. 
Erzeugen der Applikation durch Build All.




