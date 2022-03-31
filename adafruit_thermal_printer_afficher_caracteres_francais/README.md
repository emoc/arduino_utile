# Caractères accentués sur imprimante thermique

Pour ce modèle de mini-imprimante thermique :

https://learn.adafruit.com/mini-thermal-receipt-printer?view=all

![adafruit thermal printer](./assets/adafruit_mini_thermal_printer.jpg?0)

## Comment utiliser les caractères accentués ?

Il faut d'abord régler CHARSET et CODEPAGE

```
(...)
printer.setCharset(CHARSET_FRANCE);
printer.setCodePage(CODEPAGE_CP437);
(...)
```

On peut trouver le tableau du codepage CP437 à la fin de la [datasheet de l'imprimante](./assets/A2-user-manual.pdf)

![codepage CP437](./assets/codepage_cp437.png?0)

### Méthode lettre par lettre fastidieuse

En remplaçant les caractères accentués par l'octet correspondant dans le tableau codepage.

ex, pour écrire «Maÿliss»
```
printer.write(0xAE);
printer.write(0x4D);
printer.write(0x61);
printer.write(0x98);
printer.write(0x6C);
printer.write(0x69);
printer.write(0x73);
printer.write(0x73);
printer.write(0xAF);
```
Les caractères [ASCII](https://en.wikipedia.org/wiki/ASCII) (*ASS-kee*) peuvent aussi être écrits directement
```
printer.write(0xAE);
printer.print("Ma");
printer.write(0x98);
printer.print("liss");
printer.write(0xAF);
```

### Méthode mieux

Plus générale, en utilisant une fonction qui filtre les chaînes de caractères et envoie des octets avec `serial.write()`

(NB : améliorable : seuls les caractères les plus communs sont transformés, tout le code page n'y est pas! Les ligatures ne sont pas traitées, entre autre)  
(NBB : mieux vaudrait utiliser le code page CP850, qui comprend plus de caractères accentuées)  

```c
// Imprimer les caractères accentués du français
// Sur une mini imprimante thermique Adafruit

String test = "ÇüéâäàçêëèïîÉôöûÿ";

void setup() {
  Serial.begin(9600);
}
void loop() {

  // Les caractères accentués sont stockés sur 2 octets
  // La longueur de chaîne renvoie le nombre d'octets
  // Serial.print("longueur de chaîne : ");
  // Serial.println(test.length());

  imprimerAvecCaracteresSpeciaux(test);

  delay(100000);
}

void imprimerAvecCaracteresSpeciaux(String test) {
  int i = 0;
  boolean special = false;
  byte printable;

  while (i < test.length()) {

    if (byte(test[i]) == 0xC3) { // Caractère spécial détecté
      special = true;
      byte carac = test[i + 1];

      // Serial.print(byte(test[i]), HEX);
      // Serial.print(" ");
      // Serial.println(carac, HEX);

      i += 2;

      switch (carac) {
        case 0x87 :  printable = 0x80;  break;  // Ç
        case 0xBC :  printable = 0x81;  break;  // ü
        case 0xA9 :  printable = 0x82;  break;  // é
        case 0xA2 :  printable = 0x83;  break;  // â
        case 0xA4 :  printable = 0x84;  break;  // ä
        case 0xA0 :  printable = 0x85;  break;  // à
        case 0xA7 :  printable = 0x87;  break;  // ç
        case 0xAA :  printable = 0x88;  break;  // ê
        case 0xAB :  printable = 0x89;  break;  // ë
        case 0xA8 :  printable = 0x8A;  break;  // è
        case 0xAF :  printable = 0x8B;  break;  // ï
        case 0xAE :  printable = 0x8C;  break;  // î
        case 0x89 :  printable = 0x90;  break;  // É
        case 0xB4 :  printable = 0x93;  break;  // ô
        case 0xB6 :  printable = 0x94;  break;  // ö
        case 0xBB :  printable = 0x96;  break;  // û
        case 0xBF :  printable = 0x98;  break;  // ÿ
        default :    printable = 0xFA;  break;
      }
    } else {                    // ASCII de base
      printable = byte(test[i]);
      i += 1;
    }
    Serial.write(printable);
  }
}
```

Pour qui ?  
Pour toutes les personnes qui utilisent une imprimante thermique.  
Pour toutes les personnes francophones qui utilisent une mini-imprimante thermique.  
Pour toutes les personnes francophones qui utilisent une mini-imprimante thermique avec un arduino et sont sensibles à l'accentuation.  

