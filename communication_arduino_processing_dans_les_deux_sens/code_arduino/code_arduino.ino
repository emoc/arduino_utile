/* 
    communication bilatérale entre arduino et processing
    arduino <-> processing
    les données des 4 capteurs sont envoyées d'arduino vers processing
    des caractères simples sont envoyés de processing vers arduino
*/

#define BROCHE_POTENTIOMETRE     A0
#define BROCHE_RESISTANCEVAR     A1
#define BROCHE_PHOTORESISTANCE   A2
#define BROCHE_THERMISTOR        A3
#define BROCHE_LED               13

int signal_potentiometre;              
int signal_resistancevar;
int signal_photoresistance;
int signal_thermistor;

int inByte = 0;                         // octet entrant

void setup() {
  pinMode(BROCHE_POTENTIOMETRE, INPUT); // la broche est définie comme une entrée
  pinMode(BROCHE_RESISTANCEVAR, INPUT); // la broche est définie comme une entrée
  pinMode(BROCHE_PHOTORESISTANCE, INPUT); // la broche est définie comme une entrée
  pinMode(BROCHE_THERMISTOR, INPUT);    // la broche est définie comme une entrée
  pinMode(BROCHE_LED, OUTPUT);
  digitalWrite(BROCHE_LED, LOW);
  
  Serial.begin(9600);                   // démarrer la communication avec l'ordinateur

  while (!Serial) {
    ; // attendre que la connection série soit réalisée
  }

}

void loop() {

  // Recevoir un caractère par le port série
  
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if ( char(inByte) == 'A') digitalWrite(BROCHE_LED, HIGH);
    if ( char(inByte) == 'B') digitalWrite(BROCHE_LED, LOW);
  }

  // Récupérer les valeurs
  
  signal_potentiometre = analogRead(BROCHE_POTENTIOMETRE);
  signal_resistancevar = analogRead(BROCHE_RESISTANCEVAR);
  signal_photoresistance = analogRead(BROCHE_PHOTORESISTANCE);
  signal_thermistor = analogRead(BROCHE_THERMISTOR);

  // Envoyer les valeurs sur le port série

  Serial.print(signal_potentiometre);
  Serial.print(",");
  Serial.print(signal_resistancevar);
  Serial.print(",");
  Serial.print(signal_photoresistance);
  Serial.print(",");
  Serial.println(signal_thermistor);

  delay(50);  // pause de 50 millisecondes

}



