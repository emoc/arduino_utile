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
            Mapping des boutons

      Select : 9       Start : 10

         7         3         8


         1  <-          ->   4


         5         2         6

   * *************************************/

  int bouton_up = JoyEvents.button_up_id;
  int bouton_down = JoyEvents.button_down_id;

  etat_bouton[bouton_up] = 0;
  etat_bouton[bouton_down] = 1;

  boolean changement_etat = false;
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

      //printEtatGlobalBouton();
      //etat_bouton_avant[n] = etat_bouton
      changement_etat = true;
    }
  }

  if (changement_etat) {
    printEtatGlobalBouton();
    for (int n = 0; n < 11; n++) {
      etat_bouton_avant[n] = etat_bouton[n];
    }
  }
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

