# Applikationsbeschreibung Schaltaktor

Die Applikation Schaltaktor erlaubt eine Parametrisierung von Schaltkanälen mit der ETS.

## **Verwendete Module**

Der Schaltaktor verwendet weitere OpenKNX-Module, die alle ihre eigene Dokumentation besitzen. Im Folgenden werden die Module und die Verweise auf deren Dokumentation aufgelistet.

### **OpenKNX**

Dies ist eine Seite mit allgemeinen Parametern, die unter [Applikationsbeschreibung-Common](https://github.com/OpenKNX/OGM-Common/blob/v1/doc/Applikationsbeschreibung-Common.md) beschrieben sind. 

### **Konfigurationstransfer**

Der Konfigurationstransfer erlaubt einen

* Export von Konfigurationen von OpenKNX-Modulen und deren Kanälen
* Import von Konfigurationen von OpenKNX-Modulen und deren Kanälen
* Kopieren der Konfiguration von einem OpenKNX-Modulkanal auf einen anderen
* Zurücksetzen der Konfiguration eines OpenKNX-Modulkanals auf Standardwerte

Die Funktionen vom Konfigurationstranfer-Modul sind unter [Applikationsbeschreibung-ConfigTransfer](https://github.com/OpenKNX/OFM-ConfigTransfer/blob/v1/doc/Applikationsbeschreibung-ConfigTransfer.md) beschrieben.

### **Logiken**

Wie die meisten OpenKNX-Applikationen enthält auch die Schaltaktor-Applikation ein Logikmodul.

Die Funktionen des Logikmoduls sind unter [Applikationsbeschreibung-Logik](https://github.com/OpenKNX/OFM-LogicModule/blob/v1/doc/Applikationsbeschreibung-Logik.md) beschrieben.

## **Schaltaktor**

<!-- DOC HelpContext="Dokumentation" -->
Mit diesem Modul können Schaltaktorkanäle parametrisiert werden.

### **Hardware**

In diesem Bereich wird die verwendete Hardware ausgewählt.

<!-- DOC -->
#### **Hardware-Variante**

Basierend auf dieser Auswahl werden einige Konfigurationsoptionen ein- bzw. ausgeblendet.

### **Kanaldefinition**

<!-- DOC -->
#### **Beschreibung des Kanals**

Der hier angegebene Name wird an verschiedenen Stellen verwendet, um diesen Kanal wiederzufinden.

* Seitenbeschreibung des Kanals: Die drei Punkte **...** werden ersetzt.
* Name vom Kommunikationsobjekt: Der Text wird hinter den Kanalnamen angehängt.

Eine aussagekräftige Benennung erlaubt eine einfachere Orientierung innerhalb der Applikation, vor allem wenn man viele Kanäle nutzt.

<!-- DOC -->
#### **Kanalaktivität**

Hier kann man einen PM-Kanal aktivieren.

##### **Inaktiv**

Dieser Kanal ist inaktiv. Alle Einstellungen und alle Kommunikaitonsobjekte sind ausgeblendet.

##### **Aktiv**

Dieser Kanal ist aktiv und kann normal parametrisiert werden.

##### **Funktionslos**

Dieser Kanal ist inaktiv. Er kann vollständig definiert sein und keine Einstellung geht verloren, aber es wird kein Telegramm empfangen oder gesendet. Dies bietet die Möglichkeit, zu Testzwecken einen bereits parametrierten Kanal inaktiv zu setzen, um zu schauen, ob er die Ursache für eventuelles Fehlverhalten im Haus ist. Kann zur Fehlersuche hilfreich sein.

<!-- DOC -->
#### **Synchron schalten**

Hier kann ein Kanal gewählt werden, mit welchem der aktuelle Kanal synchron geschaltet werden soll. Dabei ist es möglich immer drei hintereinander liegende Kanäle synchron schalten zu lassen.

### **Schaltfunktionen**

<!-- DOC -->
#### **Betriebsmodus**

Hier wird der Betriebsmodus des Schaltkanals festgelegt:

- Schließer (Standard): Das Relais ist standardmäßig offen und wird bei aktiviertem Kanal (= 1) geschlossen.
- Öffner: Das Relais ist standardmäßig geschlossen und wird bei aktiviertem Kanal (= 1) geöffnet.

<!-- DOC -->
#### **Einschaltverzögerung**

Die Zeit nach einem Einschaltsignal bis die Schaltaktion tatsächlich ausgeführt wird kann hier festgelegt werden.

<!-- DOC -->
#### **Ausschaltverzögerung**

Die Zeit nach einem Ausschaltsignal bis die Schaltaktion tatsächlich ausgeführt wird kann hier festgelegt werden.

<!-- DOC -->
#### **Zentralfunktion**

Auswahl, ob dieser Kanal bei der Zentralfunktion teilnimmt.

### **Status**

Einstellungen zum Status-Verhalten.

<!-- DOC -->
#### **Status senden**

Hier wird festgelegt, ob der Status bei Änderung gesendet werden soll.

<!-- DOC -->
#### **Zusätzlicher invertierter Status**

Auch das Aktivieren eines zusätzlichen, invertierten Status' ist möglich.

<!-- DOC -->
#### **Status zyklisch senden**

Der Status kann bei Bedarf auch zyklisch gesendet werden.

### **Verhalten**

Das Verhalten des Schaltkanals bei verschiedenen Ereignissen wird im Folgenden festgelegt.

<!-- DOC -->
#### **bei Sperren**

Eine optional Schaltaktion bei Sperrung des Kanals kann hier festgelegt werden.

<!-- DOC -->
#### **bei Entsperren**

Eine optional Schaltaktion bei Entsperrung des Kanals kann hier festgelegt werden.

<!-- DOC -->
#### **bei Busspannungsverlust**

Eine optional Schaltaktion bei Busspannungsverlust kann hier festgelegt werden.

<!-- DOC -->
#### **bei Busspannungswiederkehr**

Eine optional Schaltaktion bei Busspannungswiederkehr kann hier festgelegt werden.

### **Zusatzfunktionen**

Zusätzliche Funktionen stehen hier zur Verfügung.

<!-- DOC -->
#### **Messungen aktivieren**

Festlegung, ob die Funktionen zu Messungen genutzt werden sollen.

<!-- DOC -->
#### **Szenen aktivieren**

Festlegung, ob Szenenfunktionen genutzt werden sollen.

### **Szene**

Hier finden sich Einstellungen zur jeweiligen Szene.

<!-- DOC -->
#### **Szene aktiv**

Festlegung, ob die gewählte Szene aktiv ist.

<!-- DOC -->
#### **Szene Nummer**

Auswahl der Szenennummer, auf die reagiert werden soll.

Werden mehrere Szenen aktiviert und dieselbe Szenennummer zugewiesen, wird lediglich das Verhalten der ersten aktivierten Szene der jeweiligen Szenennummer berücksichtigt.

<!-- DOC -->
#### **Szene Verhalten**

Das gewünschte Verhalten der Szene kann hier gewählt werden.

