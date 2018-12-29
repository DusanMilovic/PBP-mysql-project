#include "planerOdmora.h"

void glavniMeni(Sql *sql);

void dodajKorisnika(Sql *sql);
void dodajAdmina(Sql *sql);
void zapocniOdmor(Sql *sql);
void dodajOdmor(Sql *sql, char *emailKorisnika, char *noteKorisnika, char *datumOdmora);
void dodajAlternativniOdmor(Sql *sql, char *primarniOdmor, char *emailKorisnika);
void odmorUBloku(Sql *sql, char *emailKorisnika, char *datumOdmora);
void dodajPrimarniSekundarniOdmor(Sql *sql, char *emailKorisnika, char *primarniOdmor, char *alternativniOdmor);
void dodajOdgovorNaZahtevOdmora(Sql *sql);
void ispisiSveKorisnike(Sql *sql);
void ispisiSveAdmine(Sql *sql);
void ispisSveOdgovoreneOdmore(Sql *sql, char *adminEmail, char *korisnikEmail);
void ispisiSveZahteveZaOdmor(Sql *sql, char *adminEmail, char *korisnikEmail);
void odgovoriNaOdmor(Sql *sql, char *adminEmail, char *korisnikEmail, char *datumOdmora);
void ispisiLiveStatistic(Sql *sql, char *korisnikEmail);
void ispisiPracenjeDana(Sql *sql, char *korisnikEmail);
void dodajDane(Sql *sql);
void azurirajLiveStatistic(Sql *sql, char *emailKorisnika, int brojDana);
void unosMejlaZaIspisPracenjeDana (Sql *sql);
void unosMejlaZaIspisAktivneStatistike(Sql *sql);