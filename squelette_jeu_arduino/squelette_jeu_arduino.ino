/* Squelette de jeu pour arduino
 
    Workshop Arcade & Craft & Alt Ctrl @ Ecole de Design Nantes Atlantique
    21 / 25 fév. 2022 / pierre@lesporteslogiques.net

    Différentes éptapes pour un jeu électronique
    Utilise une led (broche 9) et un bouton (broche 8), pour la démo uniquement!
    Bibliothèques à installer depuis le gestionnaire :
      + lib. OneButton v2.0.4 de Matthias Hertel https://github.com/mathertel/OneButton 
*/

// Inclure les bibliothèques 

#include "OneButton.h"

// Définir le circuit

#define BROCHE_LED       9          // Retour visuel pour voir dans quelle étape le jeu se trouve
#define BROCHE_BOUTON    8          // Changements d'étapes déclenchées par le bouton

OneButton bouton(BROCHE_BOUTON, false, false);

// Variables du jeu

int etape_jeu = 0;                  // Quelle étape ? attract mode, début de partie, etc.
long demarrage_partie;              // Moment de démarrage d'une nouvelle partie (millis)
int score = 0;                      // Score


void setup() {
  pinMode(BROCHE_LED, OUTPUT);      // Sortie pour activer la led
  pinMode(BROCHE_BOUTON, INPUT);    // Entrée pour capter le bouton

  // Associer une fonction de callback qui sera déclenchée à chaque clic sur le bouton
  bouton.attachClick(boutonClic);   
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
    etape_jeu = 2;                  // ... et on passe à la suite 
  } // *** fin de etape_jeu = 1 *****************************************


  if (etape_jeu == 2) {             // Partie en cours ******************
    // Le jeu se déroule ...
    clignoterLed(100,0);            // La led reste fixe
  } // *** fin de etape_jeu = 2 *****************************************
  


  if (etape_jeu == 3) {             // Fin de partie *******************
    // Le jeu se termine : on annonce win et lose, etc.
    clignoterLed(20, 480);          // La led flashe
  } // *** fin de etape_jeu = 3 *****************************************

}

// Réagir au clic sur le bouton

void boutonClic() {
  etape_jeu ++;
  if (etape_jeu > 3) etape_jeu = 0;
}

// Fonction pour obtenir des clignotements de led différents

void clignoterLed(int ledon, int ledoff) {
  int duree = ledon + ledoff;
  if (millis() % duree < ledon) digitalWrite(BROCHE_LED, HIGH);
  else digitalWrite(BROCHE_LED, LOW);  
}

