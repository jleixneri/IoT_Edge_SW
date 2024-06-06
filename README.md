# IoT_Edge_SW
IoT-Edge-SW Entwickler Bewerber Chalange

## Beschreibung 
Die Applikation simmuliert das Senden einer Nachricht von einem IoT Client an einen Azure IoTHub. Zu diesem Zweck wird eine Json Nachricht aus einem File geladen 
und eine entsprechende Nachricht generiert. Über den Client wird diese Nachricht via MQTT an einen Azure IoTHub gesendet.

Des weiteren soll eine "Direct Method" am IoTHub eingerichtet werden, die eine Nachricht an den Client sendet. Ein entsprechder Callback ist am Client implementiert.

## Abhängigkeiten
Für die Erzeugung der Applikation ist der Download der folgenden Libraries notwendig (Folgen Sie den Instruktionen auf den entsprechenden Github Seiten):

* rapidjson
  * git clone https://github.com/Tencent/rapidjson
  * cd rapidjson
  * git submodule update --init
  * Header only Library - daher ist der Clone ausreichend

* azure-iot-sdk-c
  * git clone https://github.com/Azure/azure-iot-sdk-c.git
  * cd azure-iot-sdk-c
  * git checkout LTS_07_2022_Ref01
  * git submodule update --init --recursive
  * mkdir cmake
  * cd cmake
  * make ..
  * cmake --build .
 
## Einrichten eines IoTHubs in Azure
Am Azure Poral muss ein IoTHub erzeugt werden. Dazu sind folgende Schritte erforderlich:
* Öffnen des Azure Portals
* Zum Erzeugen eines IoT Hubs und eines Gerätes folgen Sie den Schritten in https://learn.microsoft.com/de-de/azure/iot-hub/iot-hub-create-through-portal
* Richten Sie eine 'Direkte Methode' mit dem Namen 'Test' und einem entsprechenden Payload ein (JSON!!) 

## Download des Projekts und im Visual Studio 2022 anzeigen
Klonen des Projekts mit 'cmd' oder 'Git Bash' mit 
git clone https://github.com/jleixneri/IoT_Edge_SW.git

Öffnen des Visual Studios und 'Öffnen eines lokalen Ordners' anklicken. Ordner in dem der Klone geladen wurde öffnen. 
Default Config ist x64 Debug.

Die Pfade der beiden Libraries im File CMakeLists.txt müssen angepasst werden:
* set (AZURE_IOT_SDK_C_DIR "C:/Users/jleix/source/repos/Library/azure-iot-sdk-c")
* set (RAPID_JSON_ROOT "C:/Users/jleix/source/repos/Library/rapidjson")

Anpassen des Connection Strings in AzureIoTHubClient.cpp
* In der Azure Plattform finden Sie auf der Geräte Seite den Eintrag 'Primäre Verbindungszeichenfolge'. Kopieren Sie diesen in den
  static const char* connectionString = [ your connection string];

Erzeugen der Applikation durch Build All.


## Tests
Vorbereitungen:
* Azure Plattform öffnen
* Cloud Shell öffnen und 
	* az extension add --name azure-iot
	* az iot hub monitor-events --hub-name IotHub01
	
Wenn die Applikation gestartet wird sollte die von der Applikation gesendete Nachricht auf der Shell angezeigt werden.

Abschließend die eingereichtete 'Direkte Methode' mit 'Methode aufrufen' aufrufen.


## Weitere Anmerkungen
Eine volle Dokumentation finden Sie im File 'HowToDo.pdf'



