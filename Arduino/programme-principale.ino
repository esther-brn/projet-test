  // PROGRAMME ARDUINO Projet capteur 4A Génie Physique
  // Esther BOIRON
  // Margot HERNANDEZ

  ////////////////////  OLED
  #include <Adafruit_SSD1306.h>
  #include <Fonts/FreeMono9pt7b.h>
  #define largeur_ecran 128
  #define hauteur_ecran 64
  #define reset_ecran -1
  #define adresseI2C_OLED 0x3C
  Adafruit_SSD1306 ecran_OLED(largeur_ecran, hauteur_ecran, &Wire, reset_ecran);

  ////////////////////  BLUETOOTH
  #include <SoftwareSerial.h>
  #include <avr/pgmspace.h>         // Permet de stocker des données en mémoire flash (PROGMEM) au lieu de la RAM
  #include <SPI.h>
  #define rxPin 8
  #define txPin 9
  #define SERIAL
  SoftwareSerial mySerial(rxPin, txPin);
  int mode_app = 0;


  ////////////////////  CAPTEURS ET RESISTANCES
  const int pin_capteur_graphite = A0;
  const int pin_capteur_commercial = A1;
  const float R1 = 100000.0; 
  const float R3 = 100000.0; 
  const float R4 = 10000.0; 
  const float R6 = 33000.0; 
  const float Vcc = 5.0;
  float R2 = 515.0;             // Valeur correspondant à la position du potentiomètre digital

  //////////////////// POTENTIOMETRE DIGITAL
  const byte csPin           = 10;      // MCP42100 chip select pin
  const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
  const long rAB             = 50000;   // valeur à remplacer par la valeur réelle
  const byte rWiper          = 125;     // 125 ohms pot wiper resistance
  const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
  const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001 

  ////////////////////  ENCODEUR ROTATIF
  #define encoder0PinA 2
  #define encoder0PinB 3
  #define pin_bouton 4
  volatile int encoder0Pos = 0;
  const char menus[][20] PROGMEM = {"informations", "application", "capteur graphite", "capteur commercial"};    // liste des menus stockée en mémoire flash au lieu de la RAM
  const int nb_menus = sizeof(menus) / sizeof(menus[0]);
  int menu_index = 0, menu_etat = 0;
  long position_precedente = -999;

  ///////////////////// SET UP POTENTIOMETRE
  void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limite les positions entre 0 et 255
  digitalWrite(csPin, LOW);                // enable chip select
  SPI.transfer(addr);                      
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // disable chip select
}
  void setup() {

      digitalWrite(csPin, HIGH);        // chip select default to de-selected
      pinMode(csPin, OUTPUT);           // configure chip select as output
      SPI.begin();
      pinMode(rxPin, INPUT);
      pinMode(txPin, OUTPUT);
      mySerial.begin(9600);
      Serial.begin(9600);

      if (!ecran_OLED.begin(SSD1306_SWITCHCAPVCC, adresseI2C_OLED)) 
      {
        while (1);  
      }
      ecran_OLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecran_OLED.clearDisplay();
      ecran_OLED.setTextSize(1);
      ecran_OLED.setCursor(0, 0);
      ecran_OLED.setFont(&FreeMono9pt7b);
      ecran_OLED.print(F("\n Projet\n capteur\n 4A GP"));
      ecran_OLED.display();
      delay(1000);
      ecran_OLED.setFont();
      afficher_menu();

      pinMode(encoder0PinA, INPUT_PULLUP);
      pinMode(encoder0PinB, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, RISING);
      pinMode(pin_bouton, INPUT_PULLUP);
  }

  void loop() {

      setPotWiper(pot0,2);          // la position 2 est la position optimale d'après nos tests
      long position = encoder0Pos;
      if (menu_etat == 0) {        // Navigation dans le menu principal uniquement si menu_etat == 0 (0 = menu principal et 1 = sous-menu sélectionné)
      if (position != position_precedente) {
        if (position > position_precedente) {
          menu_index--;
        }
        if (position < position_precedente) {
          menu_index++;
        }

        if (menu_index < 0) menu_index = nb_menus - 1;
        if (menu_index >= nb_menus) menu_index = 0;

        afficher_menu();
        position_precedente = position;  
      }    
    }

      if (digitalRead(pin_bouton) == LOW) {
          delay(200);
          if (menu_etat == 0) {    // Si on est dans le menu principal, entrer dans un sous-menu
          menu_etat = 1;
          } else {                // Si on est déjà dans un sous-menu, revenir au menu principal
            menu_etat = 0;
              afficher_menu();
          }
          delay(200);
      }

      if (menu_etat == 1) 
      {
        afficher_sous_menu();
      }
      delay(100);
  }

  void doEncoder() {
      if (digitalRead(encoder0PinA) == HIGH && digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos++;
    } else if (digitalRead(encoder0PinA) == HIGH && digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    }
  }

  void afficher_menu() {
      char buffer[20];
      strcpy_P(buffer, menus[menu_index]);
      ecran_OLED.clearDisplay();
      ecran_OLED.setCursor(0, 0);
      ecran_OLED.println(F("Menu:"));
      ecran_OLED.setCursor(0, 20);
      ecran_OLED.print(F("> "));
      ecran_OLED.println(buffer);
      ecran_OLED.display();
  }

  void afficher_sous_menu() {
      ecran_OLED.clearDisplay();
      ecran_OLED.setCursor(0, 0);
      
      if (menu_index == 0) {
          ecran_OLED.println("Menu informations :\n\n");
          ecran_OLED.println(" Projet capteur\n Esther BOIRON\n Margot HERNANDEZ\n 4A GP");

      } else if (menu_index == 1) {
            byte Vadc = analogRead(pin_capteur_graphite) / 4;
            byte ADCflex = analogRead(pin_capteur_commercial)/ 4;
            //Serial.print("ADCflex : ");
            //Serial.println(ADCflex);
            ecran_OLED.println("Menu application :\n");
            ecran_OLED.println(" RDV sur l'appli !\n");
            delay (400);
            Serial.println(mode_app);
            if (mySerial.available() > 0)
            {
              mode_app = mySerial.read();
            }

            if (mode_app == 1)
            {
                mySerial.write(Vadc);
            }
            else if (mode_app == 2)
            {
                mySerial.write(ADCflex);
            }

            while (mySerial.available()) {
              Serial.print((char) mySerial.read()) ;
            }

      } else if (menu_index == 2) {
              float Vadc = analogRead(pin_capteur_graphite) * Vcc / 1023.0;
              float Rgraphite = (Vcc/Vadc * (1+R3/R2)*R1-R1-R4)/pow(10,6);; 
              ecran_OLED.println("Menu capteur\n");
              ecran_OLED.println("graphite :\n");
              ecran_OLED.print(F(" Res: "));
              ecran_OLED.print(Rgraphite, 4);
              ecran_OLED.println(F(" MOhm"));

      } else if (menu_index == 3) {
          int ADCflex = analogRead(pin_capteur_commercial);
          float Vflex = ADCflex * Vcc / 1023.0;
          float Rflex = R6 * (Vcc / Vflex - 1.0);
          ecran_OLED.println("Menu capteur\n");
          ecran_OLED.println("commercial :\n");
            ecran_OLED.print(F(" Res: "));
            ecran_OLED.print(Rflex, 0);
            ecran_OLED.println(F(" Ohm"));
          } 

        ecran_OLED.display();
    }
