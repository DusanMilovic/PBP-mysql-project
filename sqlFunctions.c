#include "sqlFunctions.h"
#include "planerOdmora.h"

void dodajKorisnika(Sql *sql) {
	printf("Unesite: ime, prezime, email, broj telefona, grad, datum rodjenja, datum zaposljavanja\n");
	char ime[20];
	char prezime[20];
	char email[20];
	char brojTelefona[20];
	char grad[20];
	char datumRodjenja[20];
	char datumZaposljavanja[20];

	scanf("%s%s%s%s%s%s%s", &ime, &prezime, &email, &brojTelefona, &grad, &datumRodjenja, &datumZaposljavanja);
	sprintf(sql->query,"insert into korisnik (ime, prezime, email, broj_telefona, adresa, datum_rodjenja, datum_zaposljavanja) values (%ms,%ms, %ms, %ms, %ms, %ms, %ms);", ime, prezime, email, brojTelefona, grad, datumRodjenja, datumZaposljavanja);
	printf("Upit koji se izvrsava '%s'\n",sql->query);
	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj korisnika\n");
    	exit (EXIT_FAILURE);
  	}

  	free(brojTelefona);
  	free(ime);
  	free(prezime);
  	free(email);
  	free(grad);
  	free(datumZaposljavanja);
  	free(datumRodjenja);
}

void dodajAdmina(Sql *sql) {
	printf("Unesite email korisnika koga zelite da postavite kao admina:\n");
	char emailAdmina[20];
	scanf("%s", &emailAdmina);

	sprintf(sql->query, "insert into admin (korisnik_email) values (%s);", emailAdmina);

	printf("Upit koji se izvrsava '%s'\n",sql->query);
	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}
}

void dodajOdmor(Sql *sql) {

}

void dodajPrimarniSekundarniOdmor(Sql *sql){

}

void dodajOdgovorNaZahtevOdmora(Sql *sql){

}

void ispisiSveZakazaneOdmore(Sql *sql){

}

void ispisiSveKorisnike(Sql *sql){

}

void ispisiSveAdmine(Sql *sql){

}