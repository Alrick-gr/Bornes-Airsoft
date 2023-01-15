void param_spawn()
{
  lcd.clear();
  
  int temps_partie = recup_temps("Partie");
  int temps_spawn  = recup_temps("Spawn");
  Depart(tempsD);
  spawn(temps_partie, temps_spawn);
}


void param_clicker()
{
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("      Clicker      ");
  lcd.setCursor(0, 1);
  lcd.print("     Limite :       ");
  lcd.setCursor(0, 2);
  lcd.print("         __         ");
  char limite[2] = {'_', '_'};
  int index = 0;
  lcd.setCursor(8, 3);
  lcd.print(" ^ ");
  
  logos();
  char key = keypad.getKey();

  while (key != '#' || limite[0] == '_' || limite[1] == '_')
  {
    key = keypad.getKey();
    if (key)
    {
      if (key == '*') menu();
      if (key != '#')
      {
        limite[index] = key;
        index++;
        if (index > 1)index = 0;

        lcd.setCursor(9, 2);
        lcd.print(limite[0]);
        lcd.print(limite[1]);

        lcd.setCursor(8 + index, 3);
        lcd.print(" ^ ");
      }
    }
  }
  Depart(tempsD);
  clicker((limite[0]  - '0') * 10 + (limite[1]  - '0'));

}

void param_CS()
{
  CS(recup_temps("Temps"));
}
void param_capture()
{
  uint8_t nbr_equipe = choix_chiffre("      Equipes :     ", 1, 7, 2);
  int temps_partie = recup_temps("Partie");
  Depart(tempsD);
  capture(nbr_equipe, temps_partie, 0);
}

void param_conquete()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Vie defence     ");
  lcd.setCursor(0, 1);
  lcd.print("     Limite :       ");
  lcd.setCursor(0, 2);
  lcd.print("         __         ");
  char limite[2] = {'_', '_'};
  uint8_t index = 0;
  lcd.setCursor(8, 3);
  lcd.print(" ^ ");
  
  logos();
  char key = keypad.getKey();

  while (key != '#' || limite[0] == '_' || limite[1] == '_')
  {
    key = keypad.getKey();
    if (key)
    {
      if (key == '*') menu();
      if (key != '#')
      {
        limite[index] = key;
        index++;
        if (index > 1)index = 0;

        lcd.setCursor(9, 2);
        lcd.print(limite[0]);
        lcd.print(limite[1]);

        lcd.setCursor(8 + index, 3);
        lcd.print(" ^ ");
      }
    }
  }
  uint8_t vie = (limite[0]  - '0') * 10 + (limite[1]  - '0');
  int temps_partie = recup_temps("Partie");
  int temps_spawn  = recup_temps("Spawn");

  Depart(tempsD);
  conquete(vie , temps_partie, temps_spawn);
}


void param_bombe()
{
  lcd.clear();
  int temps = recup_temps("Temps ?");
  int8_t code[4];
  choix_code("Code :",code, 4);
  Depart(tempsD);
  bombe(temps, code);
}

void param_duel()
{ 
  alarme = true;
  uint8_t nbr_bip = choix_chiffre("Bip avant tir :", 0, 10, 2);
  Depart(tempsD);
  duel(nbr_bip);
}

void param_scenar_dim()
{
  int temps_partie = recup_temps("Temps bombe");
  int8_t code[6];
  choix_code("Code :",code, 6);
  scenar_dim(temps_partie, code);
}
