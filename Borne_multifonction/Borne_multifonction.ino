#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
//#include "parametre.h"

#define ROUGE 11
#define VERT 13
#define BLEU 12
#define ALARME 10


#define ROW_NUM 4   //four rows
#define COLUMN_NUM 4 //four columns

//char keys[ROW_NUM][COLUMN_NUM] = {
//  {'1', '4', '7', '*'},
//  {'2', '5', '8', '0'},
//  {'3', '6', '9', '#'},
//  {'A', 'B', 'C', 'D'}
//};

char keys[ROW_NUM][COLUMN_NUM] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

byte cloche[8] = {
  B00000,
  B00100,
  B01010,
  B01010,
  B01010,
  B11111,
  B00100,
  B00000
};

byte fleche[8] = {
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
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

bool lumiere = true;
bool LEDs = true;
bool alarme = false;
bool depart = true;
int tempsD = 0;
void setup()
{
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.createChar(0, cloche);
  lcd.createChar(1, fleche);

  lcd.setCursor(7, 1);         // move cursor to   (0, 0)
  lcd.print("SAT37");        // print message at (0, 0)
  delay(1000);

  pinMode(ROUGE, OUTPUT);
  digitalWrite(ROUGE, LOW);
  pinMode(VERT, OUTPUT);
  digitalWrite(VERT, LOW);
  pinMode(BLEU, OUTPUT);
  digitalWrite(BLEU, LOW);
  pinMode(ALARME, OUTPUT);
  digitalWrite(ALARME, HIGH);

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
    switch (keypad.getKey())
    {
      case ('A'):
        switch (page)
        {
          case (1):
            param_spawn();
            break;
          case (2):
            param_CS();
            break;
          case (3):
            break;
          case (4):
            tempsD = recup_temps("Temps avant depart");
            actu_menu(4);
            break;
        }
        break;
      case ('B'):
        switch (page)
        {
          case (1):
            param_clicker();
            break;
          case (2):
            param_duel();
            break;
          case (3):
            break;
          case (4):
            couleur_LED();
            break;
        }
        break;
      case ('C'):
        switch (page)
        {
          case (1):
            param_capture();
            break;
          case (2):
            param_bombe();
            break;
          case (3):
            break;
          case (4):
            param_generaux();
            break;
        }
        break;
      case ('D'):
        switch (page)
        {
          case (1):
            param_conquete();
            break;
          case (2):
            break;
          case (3):
            break;
          case (4):
            on_alarme(true);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Ca sonne");
            while (!keypad.getKey());
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
      lcd.print("D:");
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

  if (alarme)
  {
    lcd.setCursor(19, 0);
    lcd.write(0);
  }
}


void fin_partie(String message)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Fin de partie    ");
  lcd.setCursor(0, 1);
  lcd.print(message);

  on_alarme(true);
  while (!keypad.getKey())
  {
    if (millis() / 100 % 2)couleur(random(2), random(2), random(2));
  }
  on_alarme(false);
  couleur(0, 0, 0);
  while (keypad.getKey() != '#');
  actu_menu(1);
}

void param_generaux()
{
  lcd.clear();
  char key = keypad.getKey();
  actu_param();
  while (key != '#')
  {
    key = keypad.getKey();
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
  char key = keypad.getKey();
  while (key != '#')
  {
    key = keypad.getKey();
    if (key)
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
