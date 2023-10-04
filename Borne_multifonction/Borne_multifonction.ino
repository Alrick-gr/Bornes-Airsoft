#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "I2CKeyPad.h"

#define DEBUG

#define KEY_PRESS_INT_PIN 2


#define ROUGE 11
#define VERT 13
#define BLEU 12
#define ALARME 10

const uint8_t KEYPAD_ADDRESS = 0x23;

I2CKeyPad keyPad(KEYPAD_ADDRESS);

const byte cloche[8] = {
  B00000,
  B00100,
  B01010,
  B01010,
  B01010,
  B11111,
  B00100,
  B00000
};

const byte fleche[8] = {
  B11000,
  B11100,
  B10110,
  B10011,
  B10011,
  B10110,
  B11100,
  B11000
};

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows


bool lumiere = true;
bool LEDs = true;
bool alarme = false;
bool depart = true;
int tempsD = 0;

char key = "";
bool key_pressed = false;

void handleKeyPress() {
  key_pressed = true;
}

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  keyPad.begin();
  keyPad.setKeyPadMode();
  char keyMap[] = "123A456B789C*0#D";  // N = NoKey, F = Fail
  keyPad.loadKeyMap(keyMap);
  
  pinMode(KEY_PRESS_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(KEY_PRESS_INT_PIN), handleKeyPress, FALLING);
  
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.createChar(0, cloche);
  lcd.createChar(1, fleche);

  lcd.setCursor(7, 1);
  lcd.print("SAT37");
  
  
  pinMode(ROUGE, OUTPUT);
  digitalWrite(ROUGE, LOW);
  pinMode(VERT, OUTPUT);
  digitalWrite(VERT, LOW);
  pinMode(BLEU, OUTPUT);
  digitalWrite(BLEU, LOW);
  pinMode(ALARME, OUTPUT);
  digitalWrite(ALARME, HIGH);
  
  couleur(1,0,0);
  delay(250);
  couleur(0,1,0);
  delay(250);
  couleur(0,0,1);
  delay(250);
  couleur(0,1,1);
  delay(250);
  couleur(1,0,1);
  delay(250);
  couleur(1,1,0);
  delay(250);
  couleur(0,0,0);

  menu();
}

void loop()
{
}

void menu()
{
  uint8_t page = 1;
  actu_menu(page);
  while (true)
  {
    getKeyPad(&key);
    switch (key)
    {
      case ('A'):
        switch (page)
        {
          case (1):
            param_spawn();
            actu_menu(page);
            break;
          case (2):
            param_CS();
            actu_menu(page);
            break;
          case (3):
            break;
          case (4):
            tempsD = recup_temps("Temps avant depart");
            actu_menu(page);
            break;
        }
        break;
      case ('B'):
        switch (page)
        {
          case (1):
            param_clicker();
            actu_menu(page);
            break;
          case (2):
            param_duel();
            actu_menu(page);
            break;
          case (3):
            break;
          case (4):
            couleur_LED();
            actu_menu(page);
            break;
        }
        break;
      case ('C'):
        switch (page)
        {
          case (1):
            param_capture();
            actu_menu(page);
            break;
          case (2):
            param_bombe();
            actu_menu(page);
            break;
          case (3):
            break;
          case (4):
            param_generaux();
            actu_menu(page);
            break;
        }
        break;
      case ('D'):
        switch (page)
        {
          case (1):
            param_conquete();
            actu_menu(page);
            break;
          case (2):
            param_scenar_dim();
            actu_menu(page);
            break;
          case (3):
            break;
          case (4):
            on_alarme(true);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Ca sonne");
            while (!getKeyPad(&key));
            on_alarme(false);
            actu_menu(page);
        }
        break;
      case ('1'):
        page = 1;
        actu_menu(page);
        break;
      case ('2'):
        page = 2;
        actu_menu(page);
        break;
      case ('3'):
        page = 3;
        actu_menu(page);
        break;
      case ('4'):
        page = 4;
        actu_menu(page);
        break;
      default:
        break;
    }
  }
}

void actu_menu(uint8_t page)
{
  lcd.clear();
  switch (page)
  {
    case (1):
      lcd.setCursor(0, 0);
      lcd.print("A: Spawn");
      lcd.setCursor(0, 1);
      lcd.print("B: Clicker");
      lcd.setCursor(0, 2);
      lcd.print("C: Capture Zone");
      lcd.setCursor(0, 3);
      lcd.print("D: Conquete");
      break;
    case (2):
      lcd.setCursor(0, 0);
      lcd.print("A: CS:GO");
      lcd.setCursor(0, 1);
      lcd.print("B: Duel");
      lcd.setCursor(0, 2);
      lcd.print("C: Bombe");
      lcd.setCursor(0, 3);
      lcd.print("D: Scenario Dim");
      break;
    case (3):
      lcd.setCursor(0, 0);
      lcd.print("A:");
      lcd.setCursor(0, 1);
      lcd.print("B:");
      lcd.setCursor(0, 2);
      lcd.print("C:");
      lcd.setCursor(0, 3);
      lcd.print("D:");
      break;
    case (4):
      lcd.setCursor(0, 0);
      lcd.print("A: Depart differe");
      lcd.setCursor(0, 1);
      lcd.print("B: Couleurs");
      lcd.setCursor(0, 2);
      lcd.print("C: Parametres");
      lcd.setCursor(0, 3);
      lcd.print("D: Alarme");
      break;
  }
  lcd.setCursor(19, 3);
  lcd.print(page);

  logos();
}

void fin_partie(String message)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Fin de partie    ");
  lcd.setCursor(0, 1);
  lcd.print(message);

  char key;
  on_alarme(true);
  while (!getKeyPad(&key))
  {
    if (millis() / 100 % 2)couleur(random(2), random(2), random(2));
  }
  on_alarme(false);
  couleur(0, 0, 0);
  while (key != '#')
    getKeyPad(&key);
}

void param_generaux()
{
  lcd.clear();
  actu_param();
  while (key != '#')
  {
    if(key_pressed)
    {
      getKeyPad(&key);
    }
    switch (key)
    {
      case ('A'):
        lumiere = !lumiere;
        actu_param();
        break;

      case ('B'):
        LEDs = !LEDs;
        actu_param();
        break;

      case ('C'):
        alarme = !alarme;
        actu_param();
        break;

      case ('D'):
        depart = !depart;
        actu_param();
        break;

      default:
        break;

    }
  }
  couleur(0, 0, 0);
  actu_menu(4);
}

void actu_param()
{
  lcd.setCursor(0, 0);
  lcd.print("A: Ecran  ");
  if (lumiere)
  {
    lcd.print("ON ");
    lcd.backlight();
  }
  else
  {
    lcd.print("OFF");
    lcd.noBacklight();
  }

  lcd.setCursor(0, 1);
  lcd.print("B: LEDs   ");
  if (LEDs) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(0, 2);
  lcd.print("C: Alarme ");
  if (alarme) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(0, 3);
  lcd.print("D: Depart ");
  if (depart) lcd.print("ON ");
  else lcd.print("OFF");
}

void couleur_LED()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rouge   Vert   Bleu");
  lcd.setCursor(0, 1);
  lcd.print("Cyan   Violet  Jaune");
  lcd.setCursor(0, 2);
  lcd.print("Blanc           Noir");
  uint8_t num = 0;
  while (key != '#')
  {
    if (getKeyPad(&key))
    {
      num = key - '0';
      switch (num)
      {
        case (1): couleur(1, 0, 0);
          break;
        case (2): couleur(0, 1, 0);
          break;
        case (3): couleur(0, 0, 1);
          break;
        case (4): couleur(0, 1, 1);
          break;
        case (5): couleur(1, 0, 1);
          break;
        case (6): couleur(1, 1, 0);
          break;
        case (7): couleur(1, 1, 1);
          break;
        case (9): couleur(0, 0, 0);
          break;
      }
    }
  }
  menu();
}
bool getKeyPad(char * value)
{
  if(key_pressed && keyPad.isPressed())
  {
    key_pressed = false;
    *value = keyPad.getChar();
    #ifdef DEBUG
    Serial.println(*value);
#endif
    return 1;
  }
  else
    *value = "";
  return 0;
}
