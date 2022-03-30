/*
   communication processing <-> arduino
 */

import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

Serial myPort;
float valeur1 = 0, valeur2 = 0, valeur3 = 0, valeur4 = 0;

void setup() {
  size(1023, 400);

  cp5 = new ControlP5(this);
  cp5.addBang("on")
    .setPosition(200, 40)
    .setSize(40, 40)
    .setId(1)
    ;

  cp5.addBang("off")
    .setPosition(250, 40)
    .setSize(40, 40)
    .setId(2)
    ;
    
  printArray(Serial.list()); // IDENTIFIER LE PORT SUR LEQUEL EST RELIE ARDUINO
  String portName = Serial.list()[1]; // Port 0 dans mon cas
  myPort = new Serial(this, portName, 9600); // Ouvrir la communication sur ce port à 9600 bauds
  myPort.bufferUntil('\n');                  // la fin du message est signifiée par un retour à la ligne
}

void draw() {
  background(0);
  fill(255);
  textSize(24);
  text(valeur1, 100, 150);
  text(valeur2, 100, 200);
  text(valeur3, 100, 250);
  text(valeur4, 100, 300);
}

void serialEvent(Serial myPort) {             // Réagir en cas d'évènement série
  try {                                       // (la structure try/catch est une gestion d'erreurs)
    while (myPort.available() > 0) {                  // Si le port est disponible
      String inBuffer = myPort.readStringUntil('\n'); // Lire les données transmises jusqu'au retour à la ligne
      if (inBuffer != null) {                         // Si le message est pas vide
        try {
          String texte = inBuffer;                      // Alors mettre les données dans une variable
          println(texte);                               // Et l'afficher sur la console
          // Traiter le message
          String[] list = split(inBuffer, ",");         // Découper le message pour récupérer les données
          valeur1 = float(list[0]);                     // Transformer la donnée texte en nombre flottant
          valeur2 = float(list[1]);
          valeur3 = float(list[2]);
          valeur4 = float(list[3]);
        }
        catch(NumberFormatException npe) {
        }
      }
    }
  }
  catch(Exception e) {
  }
}

public void on() {
  println("on!");
  myPort.clear();
  myPort.write('A');
}

public void off() {
  println("off!");
  myPort.clear();
  myPort.write('B');
}
