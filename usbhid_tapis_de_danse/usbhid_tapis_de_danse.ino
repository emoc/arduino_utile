/* Utilisation d'un tapis de danse USB HID 
    Workshop Arcade & Craft & Alt Ctrl @ Ecole de Design Nantes Atlantique
    21 / 25 fév. 2022 / pierre@lesporteslogiques.net

    arduino 1.8.5 @ kirin / Debian 9.5 stretch

    Matériel
      * Arduino UNO
      * shield USB HOST (Arduino): https://www.arduino.cc/en/Main/ArduinoUSBHostShield&lang=
      
    Bibliothèques à installer depuis github :
      + lib. USB Host Shield 2.0  https://github.com/felis/USB_Host_Shield_2.0
*/

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "hidjoystickrptparser.h"

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

// Stocker 11 valeurs pour ne pas avoir à jongler avec les valeurs
int etat_bouton[]       = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int etat_bouton_avant[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay(200);

  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}

void loop() {
  Usb.Task();

  /* *************************************
     Mapping des boutons du tapis de danse

      Select : 9       Start : 10

         7         3         8


         1  <-          ->   4


         5         2         6

   * *************************************/
  /*
    JoyEvents.button_up_id; et JoyEvents.button_down_id;
    renvoient la valeur du bouton quand un évènement bouton est détecté (down = pressé, up = relaché)
    Sinon, ils renvoient 99, signifiant qu'il n'y a pas eu de changement (c'est moche mais ça fonctionne)
    l'état des boutons est conservé dans le tableau etat_bouton
    Les index correspondent aux numéros des boutons
    L'index 0 ne correspond à rien!
    etat_bouton_avant contient l'état des boutons dans la précédente itération
    donc les actions sont à effectuer en fonctiondes valeurs du tableau etat_bouton
   */

  // Récupérer les évènements de bouton
  int bouton_up = JoyEvents.button_up_id;
  int bouton_down = JoyEvents.button_down_id;

  // Mettre à jour le tableau des boutons
  if (bouton_up < 99) etat_bouton[bouton_up] = 0;
  if (bouton_down < 99) etat_bouton[bouton_down] = 1;

  boolean changement_etat = false;

  // Comparer pour voir s'il y a eu des changements
  for (int n = 0; n < 11; n++) {
    if (etat_bouton[n] != etat_bouton_avant[n]) {
      if (etat_bouton[n] == 1) {
        Serial.print("bouton pressé : ");
        Serial.println(n);
      }
      if (etat_bouton[n] == 0) {
        Serial.print("bouton relâché : ");
        Serial.println(n);
      }
      changement_etat = true;
    }
  }

  // Si l'état a changé, les états des boutons deviennent les états précédents
  if (changement_etat) {
    printEtatGlobalBouton();
    for (int n = 0; n < 11; n++) {
      etat_bouton_avant[n] = etat_bouton[n];
    }
  }

  // Actions !

  
}

void printEtatGlobalBouton() {
  Serial.print("bouton        : ");
  for (int n = 0; n < 11; n++) {
    Serial.print(etat_bouton[n]);
    if (n < 10) Serial.print(",");
  }
  Serial.println();

  Serial.print("bouton avant  : ");
  for (int n = 0; n < 11; n++) {
    Serial.print(etat_bouton_avant[n]);
    if (n < 10) Serial.print(",");
  }
  Serial.println();
}

