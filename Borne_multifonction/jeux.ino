void spawn(int temps_partie, int temps_spawn)
{
  unsigned int debut_partie = millis() / 1000;
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
      lcd.print(F("Partie : "));
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
        lcd.print(F("Spawn : "));
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
  fin_partie(F("temps ecoule"));
}

void clicker(uint8_t nbrClic)
{
  lcd.clear();
  if (nbrClic == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(F("       Clics :      "));
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
    lcd.print(F("       Vies :      "));
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
    fin_partie(F("    Plus de vies"));
  }
}

void CS(int temps, int limite)
{

#ifdef DEBUG
  Serial.print("limite val = ");
  Serial.println(limite);
#endif
  //phase plante
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("**********"));
  int8_t cpt = 0;
  int delta = 0;
  char key;
  char oldkey;
  char code[10];
  String message;

  bool fin = false;
  unsigned int avant_pose = millis() / 1000;
  while (cpt < 10 && !fin)
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
          lcd.print(F("**********"));
          cpt = 0;
          oldkey = 'A';
        }
      }
    }

    if (millis() / 500 % 2)
    {
      lcd.setCursor(7, 2);
      lcd.print(sec2temps(limite - (millis() / 1000) + avant_pose));
      if ((limite - (millis() / 1000 - avant_pose)) <= 0)
      {
        message = F("Temps ecoule");
        fin = true;
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
  lcd.print(F("**********"));
  lcd.setCursor(5, 1);
  lcd.print(F("^"));
  lcd.setCursor(5, 0);
  lcd.print(code[cpt]);
  unsigned int debut_partie = millis() / 1000;
  while (!fin)
  {
    if (millis() / 500 % 2)
    {
      if (bip)
      {
        tone(BUZZER, 4500, 100);
        bip = false;
      }

      couleur(1, 0, 0);
      lcd.setCursor(7, 2);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        message = F("Bombe explosee");
        fin = true;
      }
    }
    else
    {
      couleur(0, 0, 0);
      bip = true;
    }

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
          lcd.print(F(" ^"));
        }
        else
        {
          cpt = 9;
          lcd.setCursor(5, 0);
          lcd.print(F("**********"));
          lcd.setCursor(5, 1);
          lcd.print(F("^         "));
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

void capture(uint8_t nbr_equipe, unsigned int temps_limite, uint8_t temps_appuis, unsigned int temps_malus)
{
  lcd.clear();
  couleur(0, 0, 0);
  unsigned int debut_partie = millis() / 1000;
  unsigned int temps[nbr_equipe + 1];
  unsigned int delta_temps_malus = 0;
  unsigned int delta_appuis = -1;
  uint8_t equipe_suivante = 0;
  uint8_t equipe[nbr_equipe + 1];
  for (uint8_t i = 0; i <= nbr_equipe; i++)
  {
    temps[i] = 0;
    equipe[i] = i;
  }
  int8_t equipe_active = 0;
  bool fin = false;
  char key;
  while (!fin)
  {
    key = keypad.getKey();
    if (key)
    {
      if ((key - '0' <= nbr_equipe) && (key - '0' >= 0))
      {
        if (delta_appuis == -1)
        {
          delta_appuis = millis() / 1000;
        }
        equipe_suivante = key - '0';
      }
    }
    if (pressed) //si bouton maintenu
    {
      //Serial.println("appuis"));
      on_alarme(1);
      if (delta_appuis + temps_appuis <= millis() / 1000)
      {
        equipe_active = equipe_suivante;
        delta_temps_malus = millis() /1000;
      }
      set_couleur(((millis() / 250) % 2) ? equipe_suivante : equipe_active);
    }
    else
    {
      on_alarme(0);
      delta_appuis = -1;
      set_couleur(equipe_active);
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
      if(delta_temps_malus + temps_malus <= millis() / 1000)
      {
        equipe_active = 0;
      }
    }
    
    if (equipe_active != -1)
    {
      temps[equipe_active] = millis() / 1000 - debut_partie;
      for (uint8_t i = 0; i <= nbr_equipe; i++)
      {
        if (i != equipe_active)temps[equipe_active] -= temps[i];
      }
      if (temps[equipe_active] >= temps_limite && equipe_active != 0)fin = true;
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
  lcd.print(F("Classement"));
  delay(3000);
  lcd.clear();
  if (nbr_equipe > 4)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      lcd.setCursor(10, i);
      lcd.print(F("|"));
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
  lcd.print(F("____"));
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
        delay(500);
        //if (code[0] == entree[0] && code[1] == entree[1] && code[2] == entree[2] && code[3] == entree[3])
        if (*((uint32_t*)code) == *((uint32_t*)entree))
        {
          set_couleur(3);
          fin = true;
          message = F("Bombe desamorcee");
          alarme = false;
        }
        else
        {
          essais++;
          tone(BUZZER, 1000, 500);
          lcd.setCursor(8, 2);
          lcd.print(F("____"));
          if (essais == 3)
          {
            fin = true;
            message = F("Code faux");
          }
        }
      }
    }

    if (millis() / 500 % 2)
    {
      if (bip)
      {
        tone(BUZZER, 4500, 100);
        bip = false;
      }
      couleur(1, 0, 0);
      lcd.setCursor(7, 1);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        fin = true;
        message = F("Bombe explosee");
      }
    }
    else
    {
      couleur(0, 0, 0);
      bip = true;
    }
  }
  fin_partie(message);
}

void conquete(uint8_t nbrClick, uint32_t temps_partie, uint32_t temps_spawn)
{
  uint32_t delta = millis()/1000;
  clicker(nbrClick);
  lcd.clear();
  spawn(temps_partie - delta, temps_spawn);
}

void duel(uint8_t nbr_bip)
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(F("        DUEL      "));
  alarme = true;
  delay(5000);
  for (uint8_t i = 0; i < 2; i++)
  {
    on_alarme(true);
    delay(100);
    on_alarme(false);
    delay(1000 * random(1, 10));
  }
  fin_partie(F("TIREZ !!!"));
}

void fils(unsigned int temps, uint8_t nbr_fils)
{
  lcd.clear();
  unsigned int debut_partie = millis() / 1000;
  bool flag_actu, flag_fenetre = false;
  bool fin = false;
  String message;
  bool etat_led = true;
  char key;
  uint8_t essais, malus_fenetre, malus_delais, malus_couleur = 0;
  uint16_t delais_fenetre = 0;
  uint16_t fenetre = 0;
  for (uint8_t i = 0; i < nbr_fils; i++)
    essais += 1 << i;

  String ligne1, ligne2, ligne3, ligne4 = "";
  switch (nbr_fils)
  {
    case (6):
      ligne1 = " |";
      ligne2 = " |";
      ligne3 = " |";
      ligne4 = " A";
    case (5):
      ligne1 = " |" + ligne1;
      ligne2 = " |" + ligne2;
      ligne3 = " |" + ligne3;
      ligne4 = " B" + ligne4;
    case (4):
      ligne1 = "        |" + ligne1;
      ligne2 = "        |" + ligne2;
      ligne3 = "        |" + ligne3;
      ligne4 = "        C" + ligne4;
    case (3):
      ligne1 = " |" + ligne1;
      ligne2 = " |" + ligne2;
      ligne3 = " |" + ligne3;
      ligne4 = " 7" + ligne4;
    case (2):
      ligne1 = " |" + ligne1;
      ligne2 = " |" + ligne2;
      ligne3 = " |" + ligne3;
      ligne4 = " 4" + ligne4;
    case (1):
      ligne1 = " |" + ligne1;
      ligne2 = " |" + ligne2;
      ligne3 = " |" + ligne3;
      ligne4 = " 1" + ligne4;
  }

  lcd.setCursor(0, 0);
  lcd.print(ligne1);
  lcd.setCursor(0, 1);
  lcd.print(ligne2);
  lcd.setCursor(0, 2);
  lcd.print(ligne3);
  lcd.setCursor(0, 3);
  lcd.print(ligne4);
  while (!fin)
  {

    key = keypad.getKey();
    if (key)
    {
      lcd.setCursor(0, 0);
      switch (key)
      {
        case ('A'):
          lcd.setCursor(18, 1);
          essais = essais & (1 << 5) ? essais ^ (1 << 5) : essais;
          break;
        case ('B'):
          lcd.setCursor(16, 1);
          essais = essais & (1 << 4) ? essais ^ (1 << 4) : essais;
          break;
        case ('C'):
          lcd.setCursor(14, 1);
          essais = essais & (1 << 3) ? essais ^ (1 << 3) : essais;
          break;
        case ('7'):
          lcd.setCursor(5, 1);
          essais = essais & (1 << 2) ? essais ^ (1 << 2) : essais;
          break;
        case ('4'):
          lcd.setCursor(3, 1);
          essais = essais & (1 << 1) ? essais ^ (1 << 1) : essais;
          break;
        case ('1'):
          lcd.setCursor(1, 1);
          essais = essais & (1 << 0) ? essais ^ (1 << 0) : essais;
          break;
        default:
          break;
      }
#ifdef DEBUG
      Serial.println(essais);
#endif
      lcd.print(' ');
      if (essais == 0)
      {
        message = F("Bombe desamorcee");
        fin = true;
      }

      if (!fenetre)
      {
        switch (random(10))
        {
          case 0 :
            temps -= 60;
            break;
          case 1 :
            lcd.noBacklight();;
            break;
          case 2 :
            malus_fenetre += 15;
            break;
          case 3 :
            malus_delais += 100;
            break;
          case 4 :
            malus_couleur = true;
            break;
          default:
            break;
        }
      }
    }

    if (millis() / 100 % 2 & flag_fenetre)
    {
      flag_fenetre = false;
      if (fenetre)
      {
        fenetre--;
        couleur(0, 1, 0);
      }
      else 
      {
        if (malus_couleur)
          couleur(random(2), 0, random(2));
        else
          couleur(1, 0, 0);
      }
    }
    else
    {
      flag_fenetre = true;
    }

    if (millis() / 500 % 2 & flag_actu)
    {
      flag_actu = false;
      if (delais_fenetre == 0 && fenetre == 0)
      {
        fenetre = random(10, 50); //- malus_fenetre);
        delais_fenetre = random(100,200);// + malus_delais, 200 + malus_delais);
      }
      else if (fenetre == 0)
        delais_fenetre--;

      if (bip)
      {
        tone(BUZZER, 4500, 100);
        bip = false;
      }
      lcd.setCursor(7, 2);
      lcd.print(sec2temps(temps - (millis() / 1000) + debut_partie));
      if ((temps - (millis() / 1000 - debut_partie)) <= 0)
      {
        message = F("Bombe explosee");
        fin = true;
      }
    }
    else
    {
      flag_actu = true;
      bip = true;
    }
  }
  fin_partie(message);
}
