int recup_temps(String message)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  lcd.print("       00:00       ");
  lcd.setCursor(5, 2);
  lcd.print("  ^  ");
  
  logos();
  char key = keypad.getKey();
  uint8_t  temps[4] = {0, 0, 0, 0};
  uint8_t index = 0;
  while (key != '#')
  {
    key = keypad.getKey();
    if (key)
    {
      if (key == '*') menu();
      else
      {
        if ((key - '0' < 10) && key != '#')
        {
          temps[index] = key - '0';
          lcd.setCursor(7, 1);
          lcd.print(temps[0]);
          lcd.print(temps[1]);
          lcd.setCursor(10, 1);
          lcd.print(temps[2]);
          lcd.print(temps[3]);
          index++;

          if (index > 3) index = 0;
          if (index <= 1)lcd.setCursor(5 + index, 2);
          else lcd.setCursor(5 + index + 1, 2);
          lcd.print("  ^      ");
        }
      }
    }
  }

  return ((int)(temps[0] * 10 + temps[1]) * 60 + temps[2] * 10 + temps[3]);
}

void couleur(bool rouge, bool vert, bool bleu)
{
  if (LEDs)
  {
    digitalWrite(ROUGE, rouge);
    digitalWrite(VERT, vert);
    digitalWrite(BLEU, bleu);
  }
  else
  {
    digitalWrite(ROUGE, LOW);
    digitalWrite(VERT, LOW);
    digitalWrite(BLEU, LOW);
  }
}

void on_alarme(bool etat)
{
  if (alarme)
  {
    digitalWrite(ALARME, !etat);
  }
  else
  {
    digitalWrite(ALARME, HIGH);
  }
}


String sec2temps(int sec)
{
  uint8_t Secondes = sec % 60;
  uint8_t Minutes = (sec - Secondes) / 60;
  String Min, Sec;
  if (Secondes < 10) Sec = '0' + String(Secondes);
  else Sec = String(Secondes);

  if (Minutes < 10) Min = '0' + String(Minutes);
  else Min = String(Minutes);
  String ret = Min + ':' + Sec + ' ';
  return ret;
}

void Depart(int temps)
{
  lcd.clear();
  logos();
  lcd.setCursor(0, 0);
  lcd.print("Appuyez sur # pour");
  lcd.setCursor(0, 1);
  lcd.print("  lancer la partie");
  lcd.setCursor(0, 2);
  lcd.print("     dans " + sec2temps(temps));
  while (keypad.getKey() != '#');

  lcd.setCursor(0, 0);
  lcd.print("                   ");
  lcd.setCursor(0, 1);
  lcd.print(" debut de la partie");
  lcd.setCursor(0, 2);
  lcd.print("     dans ");
  int delta = millis() / 1000;
  while (millis() / 1000 - delta < temps)
  {
    if (millis() / 500 % 2)
    {
      lcd.setCursor(10, 2);
      lcd.print(sec2temps(temps - (millis() / 1000) + delta));
    }
  }
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Debut de partie");
  delay(3000);
  lcd.clear();
}

void logos()
{
  if (alarme)
  {
    lcd.setCursor(19, 0);
    lcd.write(0);
  }
  if(depart)
  {
    lcd.setCursor(19, 1);
    lcd.write(1);
  }
}

uint8_t choix_chiffre(String message, uint8_t minimum, uint8_t maximum, uint8_t defaut)
{
  lcd.clear();
  uint8_t nbr = 2;
  lcd.setCursor(0, 1);
  lcd.print(message);
  lcd.setCursor(9, 2);
  lcd.print(nbr);
  
  logos();

  char key;
  
  do
  {
    key = keypad.getKey();
    if (key == '*') menu();
    else
    {
      if ((key - '0' < maximum + 1) && (key - '0' > minimum-1) && key != '#')
      {
        couleur(1,0,0);
        nbr = key - '0';
        lcd.setCursor(9, 2);
        lcd.print(nbr);
        couleur(0,0,0);
      }
    }
  }
  while (key != '#');
  
  return nbr;
}

void choix_code(String message,int8_t* code, uint8_t taille)
{
  lcd.clear();
  for(uint8_t i = 0; i<taille; i++) code[i] = -1;
  uint8_t index = 0;
  char key;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
  
  lcd.setCursor((20-taille)/2,1);
  for(uint8_t i = 0; i<taille; i++)lcd.print("_");
  lcd.setCursor((20-taille)/2,2);
  lcd.print("^");

  logos();
  while(key != '#' or code[taille-1] == -1)
  {
    key = keypad.getKey();
    if (key == '*') menu();
    if(key - '0' < 10 and key - '0' >= 0)
    {
      couleur(1,0,0);
      code[index] = key - '0';
      index++;
      lcd.setCursor((20-taille)/2, 1);

      for(uint8_t i = 0; i < taille; i++)
      {
        //Serial.println(code[i]);
        if(code[i] != -1)lcd.print(code[i]);
      }
     
      if(index>=taille)index = 0;
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor((20-taille)/2 + index, 2);
      lcd.print("^");
      couleur(0,0,0);
    }
  }
}
