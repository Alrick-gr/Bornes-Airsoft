void spawn(int temps_partie, int temps_spawn)
{
  

  static int debut_partie = millis() / 1000;
  uint32_t debut_spawn = 0;
  bool flag = false;
  bool flag_temps = true;
  while (millis() / 1000 - debut_partie < temps_partie)
  {
    lcd.setCursor(8, 1);
    if ((millis() / 500) % 2 && flag_temps) //affichage du temps partie toutes les secondes
    {
      flag_temps = false;
      lcd.setCursor(0, 1);
      lcd.print("Partie : ");
      lcd.setCursor(9, 1);
      lcd.print(sec2temps(temps_partie - ((millis() / 1000) - debut_partie)));
    }
    if (!(millis() / 500 % 2))flag_temps = true;

    if (keypad.getKey() && !flag)
    {
      debut_spawn = millis() / 1000;
      flag = true;
    }
    if (flag)
    {
      if ((millis() / 1000) - debut_spawn < temps_spawn)
      {
        lcd.setCursor(0, 2);
        lcd.print("Spawn : ");
        lcd.setCursor(8, 2);
        lcd.print(sec2temps(temps_spawn - ((millis() / 1000) - debut_spawn)));
      }
      else
      {
        couleur(1, 0, 0);
        on_alarme(true);
        delay(100);
        couleur(0, 0, 0);
        on_alarme(false);
        flag = false;
        lcd.clear();
      }
    }
  }
  fin_partie("temps ecoule");
}

void clicker(uint8_t nbrClic)
{
  lcd.clear();
  if (nbrClic == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("       Clics :      ");
    lcd.setCursor(9, 2);
    lcd.print(nbrClic);
    while (true)
    {
      if (keypad.getKey())
      {
        nbrClic++;
        lcd.setCursor(9, 2);
        lcd.print(nbrClic);
      }
    }
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("       Vies :      ");
    lcd.setCursor(9, 2);
    if (nbrClic < 10) lcd.print(' ');
    lcd.print(nbrClic);
    while (nbrClic > 0)
    {
      if (keypad.getKey())
      {
        nbrClic--;

        lcd.setCursor(9, 2);
        if (nbrClic < 10) lcd.print(' ');
        lcd.print(nbrClic);
      }
    }
    fin_partie("    Plus de vies");
  }
}

void CS(int temps)
{
  //phase plante
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("**********");
  int8_t cpt = 0;
  int delta = 0;
  char key;
  char oldkey;
  char code[10];
  String message;
  while (cpt < 10)
  {
    key = keypad.getKey();
    if (key)
    {
      if (cpt <= 1)
      {
        on_alarme(true);
        delay(50);
        on_alarme(false);
      }
      if (key - '0' < 10 and key - '0' >= 0)
      {
        if (key != oldkey)
        {
          lcd.setCursor(14 - cpt, 1);
          lcd.print(key);
          code[cpt] = key;
          cpt++;
          oldkey = key;
        }
        else
        {
          lcd.setCursor(5, 1);
          lcd.print("**********");
          cpt = 0;
          oldkey = 'A';
        }
      }
    }
  }
  cpt--;
  //phase décompte
  on_alarme(1);
  delay(500);
  on_alarme(0);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("**********");
  lcd.setCursor(5, 1);
  lcd.print("^");
  lcd.setCursor(5, 0);
  lcd.print(code[cpt]);
  int debut_partie = millis() / 1000;
  bool fin = false;
  while (!fin)
  {
    if (millis() / 500 % 2)
    {
      couleur(1, 0, 0);
      lcd.setCursor(7, 2);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        message = "Bombe explosée";
        fin = true;
      }
    }
    else couleur(0, 0, 0);

    key = keypad.getKey();
    if (key)
    {
      if (cpt >= 8)
      {
        on_alarme(true);
        delay(50);
        on_alarme(false);
      }
      if (key - '0' < 10 and key - '0' >= 0)
      {
        if (key == code[cpt])
        {
          cpt--;
          lcd.setCursor(14 - cpt, 0);
          lcd.print(code[cpt]);
          lcd.setCursor(13 - cpt, 1);
          lcd.print(" ^");
        }
        else
        {
          cpt = 9;
          lcd.setCursor(5, 0);
          lcd.print("**********");
          lcd.setCursor(5, 1);
          lcd.print("^         ");
          lcd.setCursor(5, 0);
          lcd.print(code[cpt]);
        }
      }
    }
    if (cpt == -1)
    {
      fin = true;
      alarme = false;
      message = "Bombe desamorcee";
    }
  }
  fin_partie(message);
}

void capture(uint8_t nbr_equipe, int temps_limite, uint8_t difficult)
{
  lcd.clear();
  couleur(0, 0, 0);
  int debut_partie = millis() / 1000;
  int temps[nbr_equipe + 1];
  uint8_t equipe[nbr_equipe + 1];
  for (uint8_t i = 0; i <= nbr_equipe; i++)
  {
    temps[i] = 0;
    equipe[i] = i;
  }
  int8_t equipe_active = -1;
  bool fin = false;
  char key;
  while (!fin)
  {
    key = keypad.getKey();
    if (key)
    {
      if ((key - '0' <= nbr_equipe) && (key - '0' > 0))
      {
        equipe_active = key - '0';
        on_alarme(true);
        delay(200);
        on_alarme(false);
      }

      switch (equipe_active)
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
        default: couleur(0, 0, 0);
          break;
      }
    }
    if ((millis() / 500) % 2)
    {
      switch (nbr_equipe)
      {
        case (7): lcd.setCursor(6, 3);
          lcd.print("7:" + sec2temps(temps[7]));
        case (6): lcd.setCursor(13, 2);
          lcd.print("6:" + sec2temps(temps[6]));
        case (5): lcd.setCursor(0, 2);
          lcd.print("5:" + sec2temps(temps[5]));
        case (4): lcd.setCursor(13, 1);
          lcd.print("4:" + sec2temps(temps[4]));
        case (3): lcd.setCursor(0, 1);
          lcd.print("3:" + sec2temps(temps[3]));
        case (2): lcd.setCursor(13, 0);
          lcd.print("2:" + sec2temps(temps[2]));
        case (1): lcd.setCursor(0, 0);
          lcd.print("1:" + sec2temps(temps[1]));
      }
    }
    if (equipe_active != -1)
    {
      temps[equipe_active] = millis() / 1000 - debut_partie;
      //Serial.println("-----------");
      for (uint8_t i = 1; i <= nbr_equipe; i++)
      {
        if (i != equipe_active)temps[equipe_active] -= temps[i];
      }
      if (temps[equipe_active] >= temps_limite)fin = true;
    }
    else debut_partie = millis() / 1000;
  }

  fin_partie("  Equipe " + String(equipe_active) + " a gagne ");
  lcd.clear();

  int temp;
  for (uint8_t i = 1; i < nbr_equipe; i++)
  {
    if (temps[i] < temps[i + 1])
    {
      temp = temps[i];
      temps[i] = temps[i + 1];
      temps[i + 1] = temp;

      temp = equipe[i];
      equipe[i] = equipe [i + 1];
      equipe[i + 1] = temp;

      i = 0;
    }
  }
  lcd.setCursor(5, 1);
  lcd.print("Classement");
  delay(3000);
  lcd.clear();
  if (nbr_equipe > 4)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      lcd.setCursor(10, i);
      lcd.print("|");
    }
  }
  for (uint8_t i = 1; i <= nbr_equipe && i <= 7; i++)
  {

    if (i > 4) lcd.setCursor(11, i - 1 - 4);
    else lcd.setCursor(0, i - 1);

    lcd.print(String(equipe[i]) + ':' + sec2temps(temps[i]));

  }
  while (!keypad.getKey());
  actu_menu(1);
}

void bombe(int temps, uint8_t *code)
{
  lcd.clear();
  int debut_partie = millis() / 1000;
  bool fin = false;
  String message;
  bool etat_led = true;
  char key;
  uint8_t entree[4];
  uint8_t essais = 0, index = 0;
  lcd.setCursor(8, 2);
  lcd.print("____");
  while (!fin)
  {
    key = keypad.getKey();
    if (key - '0' < 10 and key - '0' >= 0)
    {
      entree[index] = key - '0';
      lcd.setCursor(8 + index, 2);
      lcd.print(entree[index]);
      index++;
      if (index == 4)
      {
        index = 0;
        couleur(1, 0, 1);
        delay(500);
        if (code[0] == entree[0] && code[1] == entree[1] && code[2] == entree[2] && code[3] == entree[3])
        {
          fin = true;
          message = "Bombe desamorcee";
          alarme = false;
        }
        else
        {
          essais++;
          couleur(1, 0, 0);
          lcd.setCursor(8, 2);
          lcd.print("____");
          if (essais == 3)
          {
            fin = true;
            message = "Code faux";
          }
        }
      }
    }

    if (millis() / 500 % 2)
    {
      couleur(1, 0, 0);
      lcd.setCursor(7, 1);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        fin = true;
        message = "Bombe explosee";
      }
    }
    else couleur(0, 0, 0);
  }
  fin_partie(message);
}

void conquete(uint8_t nbrClick, int temps_partie, int temps_spawn)
{
  clicker(nbrClick);
  lcd.clear();
  spawn(temps_partie, temps_spawn);
}

void duel(uint8_t nbr_bip)
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("        DUEL      ");
  alarme = true;
  delay(5000);
  for (uint8_t i = 0; i < 2; i++)
  {
    on_alarme(true);
    delay(100);
    on_alarme(false);
    delay(1000 * random(1, 10));
  }
  fin_partie("TIREZ !!!");
}

void scenar_dim(int temps, uint8_t *code)
{
  bool phase1 = true;
  lcd.clear();
  int debut_partie;
  bool fin = false;
  String message;
  bool etat_led = true;
  char key;
  uint8_t entree[6];
  uint8_t essais = 0, index = 0;
  lcd.setCursor(7, 2);
  lcd.print("______");
  lcd.setCursor(7, 3);
  lcd.print("RVBCJM");
  while (!fin)
  {
    key = keypad.getKey();
    switch (index)
    {
      case 0:
        couleur(1, 0, 0);
        break;
      case 1:
        couleur(0, 1, 0);
        break;
      case 2:
        couleur(0, 0, 1);
        break;
      case 3:
        couleur(0, 1, 1);
        break;
      case 4:
        couleur(1, 1, 0);
        break;
      case 5:
        couleur(1, 0, 1);
        break;
    }
    if (key - '0' < 10 and key - '0' >= 0)
    {
      entree[index] = key - '0';
      lcd.setCursor(7 + index, 2);
      lcd.print(entree[index]);
      index++;

      if (index == 6)
      {
        index = 0;
        if (code[0] == entree[0] && code[1] == entree[1] && code[2] == entree[2] && code[3] == entree[3] && code[4] == entree[4] && code[5] == entree[5])
        {
          if (phase1)
          {
            phase1 = false;
            lcd.setCursor(7, 2);
            lcd.print("______");
            for (uint8_t i = 0; i < 6; i++) entree[i] = -1;
            debut_partie = millis() / 1000;
          }
          else
          {
            fin = true;
            message = "Bombe desamorcee";
            alarme = false;
          }
        }
        else
        {
          essais++;
          couleur(1, 0, 0);
          lcd.setCursor(7, 2);
          lcd.print("______");
          if (essais == 3)
          {
            fin = true;
            message = "Code faux";
          }
        }
      }
    }

    if (millis() / 500 % 2 && !phase1)
    {
      lcd.setCursor(7, 1);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        fin = true;
        message = "Bombe explosee";
      }
    }
  }
  fin_partie(message);
}
