void param_spawn()
{
  lcd.clear();
  
  int temps_partie = recup_temps(F("Partie"));
  int temps_spawn  = recup_temps(F("Spawn"));
  Depart(tempsD);
  spawn(temps_partie, temps_spawn);
}

void param_clicker()
{
  uint8_t nbr[2];
  choix_code(F("Limite :"), nbr, 2);
  clicker(nbr[0] * 10 + nbr[1]);

}

void param_CS()
{
  unsigned int limite = recup_temps(F("Temps limite"));
  unsigned int temps = recup_temps(F("Temps explosion"));
  CS(temps, limite);
}
void param_capture()
{
  uint8_t nbr_equipe = choix_chiffre(F("      Equipes :     "), 1, 7, 2);
  unsigned int temps_partie = recup_temps(F("Partie"));
  uint8_t temps_appuis = choix_chiffre(F("   Temps appuis :   "), 0, 60, 0);
  unsigned int temps_malus = recup_temps(F("Temps malus"));
  Depart(tempsD);
  capture(nbr_equipe, temps_partie, temps_appuis, temps_malus);
}

void param_conquete()
{
  uint8_t nbr[2];
  choix_code(F("Vie defense :"), nbr, 2);
  int temps_partie = recup_temps(F("Partie"));
  int temps_spawn  = recup_temps(F("Spawn"));

  Depart(tempsD);
  conquete(nbr[0] * 10 + nbr[1] , temps_partie, temps_spawn);
}

void param_bombe()
{
  lcd.clear();
  int temps = recup_temps(F("Temps ?"));
  int8_t code[4];
  choix_code(F("Code :"),code, 4);
  Depart(tempsD);
  bombe(temps, code);
}

void param_duel()
{ 
  alarme = true;
  uint8_t nbr_bip = choix_chiffre(F("Bip avant tir :"), 0, 10, 2);
  Depart(tempsD);
  duel(nbr_bip);
}

void param_fils()
{
  fils(recup_temps(F("Partie")), choix_chiffre(F("   Nombre fils :"), 1, 6, 4));
}
