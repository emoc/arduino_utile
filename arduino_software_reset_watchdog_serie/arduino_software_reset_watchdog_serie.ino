/* Arduino software reboot à la réception d'un message série
 
   Principe :
   Quand on envoie le caractère 0 à l'arduino, il redémarre

   Comment ?
   Le watchdog est déclenché, ne recevant pas le signal approprié, il lance la procédure de reboot,
   de la même manière que si on appuyait sur le bouton de la carte.
   Il existe d'autres méthodes, mais celle-ci est la seule qui fasse un reset complet.
   
   Avertissements : 
     * cette méthode n'est pas compatible avec les anciennes versions de bootloader
     * cette méthode peut poser problème sur des clones nano (toujours pour des questions de bootloader)
       cf. https://forum.arduino.cc/index.php?topic=461948.0
       
   Testé (ok!) sur un Uno R3-DIP de Joy-it

   Sources : multiples discussions sur le forum arduino et ailleurs
     * https://www.codeproject.com/articles/1012319/arduino-software-reset
     
   Il existe aussi des libraries pour faire ça!
     * https://github.com/adafruit/Adafruit_SleepyDog 
     * https://github.com/qub1750ul/Arduino_SoftwareReset
   
  */

#include <avr/wdt.h>

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("boot");
}

void loop() {
  
  if (Serial.available() > 0) {
    char ch = Serial.read();  
    if (ch == '0') {
      Serial.println("arduino va redémarrer");
      delay(1000); 
      reboot();
    }
  }
}

void reboot() {
  wdt_enable(WDTO_15MS); // activer le watchdog
  while (1) {};          // et attendre ...
}


