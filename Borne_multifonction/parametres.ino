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
  uint8_t nbr[2];
  choix_code("Limite :", nbr, 2);
  clicker(nbr[0] * 10 + nbr[1]);

}

void param_CS()
{
  CS(recup_temps("Temps"));
}
void param_capture()
{
  uint8_t nbr_equipe = choix_chiffre("      Equipes :     ", 1, 7, 2);
  int temps_partie = recup_temps("Partie");
  uint8_t temps_appuis = choix_chiffre("   Temps appuis :   ", 0, 60, 0);
  Depart(tempsD);
  capture(nbr_equipe, temps_partie, temps_appuis);
}

void param_conquete()
{
  uint8_t nbr[2];
  choix_code("Vie defense :", nbr, 2);
  int temps_partie = recup_temps("Partie");
  int temps_spawn  = recup_temps("Spawn");

  Depart(tempsD);
  conquete(nbr[0] * 10 + nbr[1] , temps_partie, temps_spawn);
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
