/*
   code minimal pour déclencher la lecture de son
   sur un module grove serial mp3 v3 (WT2003S-20SS) et une carte microSD
   avec 2 boutons (sur 8 et 9)

   workshop objets connectés @ EDNA, 28 mars - 1er avril 2022
   pierre@lesporteslogiques.net

   arduino 1.8.5 @ kirin / debian stretch 9.5
     + lib. https://github.com/Seeed-Studio/Seeed_Serial_MP3_Player
   
   doc :
     * https://wiki.seeedstudio.com/Grove-MP3-v3/#specification

   /!\ attention aux fichiers cachés sur la carte SD
       et aux blancs en début de fichier
*/


// Inclure les bibliothèques *****************************

#include "WT2003S_Player.h"

// Définir le circuit ************************************

#define BROCHE_B1         8
#define BROCHE_B2         9

// Tout pour le son **************************************

#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3);    // RX, TX
#define COMSerial SSerial
STROAGE workdisk = SD;
WT2003S<SoftwareSerial> Mp3Player;

uint8_t volume = 20;  // volume du son



void setup() {

  pinMode(BROCHE_B1, INPUT);
  pinMode(BROCHE_B2, INPUT);

  Serial.begin(9600);
  
  // Son ***************************
  COMSerial.begin(9600);
  Mp3Player.init(COMSerial);

}


void loop() {

  if (digitalRead(BROCHE_B1) == HIGH) {
    Serial.println("bouton B1 appuyé");
    Mp3Player.volume(volume);
    delay(100);
    // On envoie l'index du fichier sur la carte pour le déclencher
    if (workdisk == SD) Mp3Player.playSDRootSong(char(1));
    delay(100);
  }
  
  if (digitalRead(BROCHE_B2) == HIGH) {
    Serial.println("bouton B2 appuyé");
    Mp3Player.volume(volume);
    delay(100);
    if (workdisk == SD) Mp3Player.playSDRootSong(char(2));
    delay(100);
  }
}


