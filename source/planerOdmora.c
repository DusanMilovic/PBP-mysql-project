#include "planerOdmora.h"
#include "sqlFunctions.h"

int main (int argc, char const *argv[])
{
  Sql sql;
  sql.connection = mysql_init(NULL);

  if (mysql_real_connect (sql.connection, "localhost", "root", "", "PlanerOdmora", 0, NULL, 0) == NULL) {
    printf ("Connection refused\n");
  } else {
    printf("Connection successful\n");
  }

  printf("Prvo cemo uneti novog korisnika sistema (nekoliko korisnika je uneto unapred, ovo je samo primer da radi dodavanje korisnika).\n");
  dodajKorisnika(&sql);

  printf("Pre rezervisanja odmora potrebno je nekoga da proglasimo za admina sistema.(Vec imamo admina ali i jednog rucno da ubacimo)");
  dodajAdmina(&sql);

  // printf("Vreme je za zakazivanje odmora!");
  // dodajOdmor(&sql);

  // printf("Neka neko od admina odgovori na zahtev odmora.");
  // dodajOdgovorNaZahtevOdmora(&sql);

  return 0;
}
