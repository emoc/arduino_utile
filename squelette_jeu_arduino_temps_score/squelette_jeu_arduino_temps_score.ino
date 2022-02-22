/* Squelette de jeu pour arduino
    Workshop Arcade & Craft & Alt Ctrl @ Ecole de Design Nantes Atlantique
    21 / 25 fév. 2022 / pierre@lesporteslogiques.net
    
    Différentes étapes de jeu, avec durée prédéfinie et affichage du score
    But: cliquer le plus possible sur le bouton pendant 5 secondes
    Utilise une led (broche 9) un bouton (broche 8) et un afficheur (4 et 5)

    arduino 1.8.5 @ kirin / Debian 9.5 stretch
    
    Bibliothèques à installer depuis le gestionnaire :
      + lib. OneButton v2.0.4 de Matthias Hertel https://github.com/mathertel/OneButton 
      + lib. SevenSegmentTM1637 v1.0.0 de Bram Hamsen https://github.com/bremme/arduino-tm1637
*/

// Inclure les bibliothèques 

#include "OneButton.h"
#include "SevenSegmentTM1637.h"

// Définir le circuit

#define BROCHE_LED       9          // Retour visuel pour voir dans quelle étape le jeu se trouve
#define BROCHE_BOUTON    8          // Bouton principal
#define BROCHE_CLK       4          // Broche Clock de l'afficheur
#define BROCHE_DIO       5          // Broche Data In Out de l'afficheur

OneButton bouton(BROCHE_BOUTON, false, false);
SevenSegmentTM1637    afficheur(BROCHE_CLK, BROCHE_DIO);

// Variables du jeu

int etape_jeu = 0;                  // Quelle étape ? attract mode, début de partie, etc.
long demarrage_partie;              // Moment de démarrage d'une nouvelle partie (millis)
long etape2_duree = 5000;           // durée du jeu en ms
long etape2_start;                  // moment où le jeu commence
long etape3_duree = 5000;           // durée de l'affichage final du score 
long etape3_start;                  // moment où l'affichage final démarre

int score = 0;                      // Score


void setup() {
  pinMode(BROCHE_LED, OUTPUT);      // Sortie pour activer la led
  pinMode(BROCHE_BOUTON, INPUT);    // Entrée pour capter le bouton

  // Associer une fonction de callback qui sera déclenchée à chaque clic sur le bouton
  bouton.attachClick(boutonClic);   
  bouton.setDebounceTicks(20);      // Réduire le temps de debounce

  // Préparer l'afficheur
  afficheur.begin();                // initialiser l'afficheur
  afficheur.setBacklight(100);      // fixer la luminosité à 100%
  afficheur.print("GO  ");          // afficher un petit message d'intro ...
  delay(1000);                      // ... pendant 1000 ms ...
  afficheur.clear();                // ... puis l'effacer
}


void loop() {

  // À l'écoute du bouton
  bouton.tick();


  if (etape_jeu == 0) {             // Attract Mode *********************
    // Ça clignote, fait du bruit pour donner envie de jouer
    clignoterLed(50, 50);         // La led clignote très rapidement
  } // *** fin de etape_jeu = 0 *****************************************



  if (etape_jeu == 1) {             // Démarrage de partie **************
    // Une nouvelle partie vient d'être déclenchée
    // Dans la démo, cette étape est très rapide ce qui ne laisse pas le temps de voir la led
    // Réinitialiser les variables pour la nouvelle partie
    score = 0;                   
    demarrage_partie = millis(); 
    etape2_start = millis();        // intialiser le début de partie
    etape_jeu = 2;                  // ... et passer à la suite 
  } // *** fin de etape_jeu = 1 *****************************************


  if (etape_jeu == 2) {             // Partie en cours ******************
    // Le jeu se déroule tant qu'on n'a pas dépassé la durée de la partie
    if (millis() - etape2_start < etape2_duree) { // jeu en cours
      afficherScore(score);         // afficher le score
      clignoterLed(100,0);          // La led reste fixe
    } else {
      etape3_start = millis();      // intialiser la fin de partie
      etape_jeu = 3;                // changer d'étape
    }
  } // *** fin de etape_jeu = 2 *****************************************
  


  if (etape_jeu == 3) {             // Fin de partie *******************
    // Le jeu se termine : on annonce win et lose pendant la durée définie
    if (millis() - etape3_start < etape3_duree) {
      clignoterLed(20, 480);        // La led flashe
      afficherScore(score);         // afficher le score final
    } else {                        // repasser en atteact mode après durée écoulée
      etape_jeu = 0;  
      afficheur.clear();
    }
  } // *** fin de etape_jeu = 3 *****************************************

}

// Réagir au clic sur le bouton

void boutonClic() {
  // 2 cas selon l'étape du jeu
  if (etape_jeu == 0) {             // cas 1 : démarrer une nouvelle partie
    etape_jeu ++;
  }
  if (etape_jeu == 2) {             // cas 2 (partie en cours) : augmenter le score
    score ++;
  }
}

// Fonction pour obtenir des clignotements de led différents

void clignoterLed(int ledon, int ledoff) {
  int duree = ledon + ledoff;
  if (millis() % duree < ledon) digitalWrite(BROCHE_LED, HIGH);
  else digitalWrite(BROCHE_LED, LOW);  
}

// Afficher le score

void afficherScore(int score) {
  //afficheur.clear();
  afficheur.print(score);
  if (etape_jeu == 3) afficheur.blink(); // clignotement en fin de partie
}

