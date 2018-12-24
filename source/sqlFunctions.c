#include "sqlFunctions.h"
#include "planerOdmora.h"

void dodajKorisnika(Sql *sql) {
	printf("\nKoliko korisnika zelite da unesete?\n");
	int n;
	scanf("%d", &n);
	printf("Unesite: ime, prezime, email, broj telefona, grad, datum rodjenja, datum zaposljavanja %d puta.\n", n);
	char ime[20];
	char prezime[20];
	char email[20];
	char brojTelefona[20];
	char grad[20];
	char datumRodjenja[20];
	char datumZaposljavanja[20];

	for (int i =0; i< n; i++) {
		scanf("%s", ime);
	    scanf("%s", prezime);
	    scanf("%s", email);
	    scanf("%s", brojTelefona);
	    scanf("%s", grad);
	    scanf("%s", datumRodjenja);
	    scanf("%s", datumZaposljavanja);

	    sprintf(sql->query,"insert into korisnik (ime,prezime,email,broj_telefona,adresa,datum_rodjenja,datum_zaposljavanja) values ('%s','%s','%s','%s','%s','%s','%s');", ime, prezime, email, brojTelefona, grad, datumRodjenja, datumZaposljavanja);
		if (i < n-1) {
			printf("Unesite podatke za narednog korisnika:\n");
		}
		if (mysql_query(sql->connection, sql->query)) {
    		printf ("Neuspesno izvrsavanje upita dodaj korisnika\n");
    		exit (EXIT_FAILURE);
  		}
	}

	glavniMeni(sql);
}

void dodajAdmina(Sql *sql) {
	printf("\nUnesite email koga zelite da postavite kao admina:\n");
	char emailAdmina[50];
	scanf("%s", emailAdmina);

	sprintf(sql->query, "insert into admin (korisnik_email) values ('%s');", emailAdmina);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}

  	glavniMeni(sql);
}

void zapocniOdmor(Sql *sql) {
	ispisiSveKorisnike(sql);
	printf("Koji korisnik (email) od ponudjenih zeli odmor: ");
	char emailKorisnika[50];
	scanf("%s", emailKorisnika);

	printf("Zasto zelite odmor: ");
	char temp;
	scanf("%c", &temp);
	char noteKorisnika[100];
	fgets(noteKorisnika, 100, stdin);

	printf("Koje godine zelite odmor: ");
	int godina;
	scanf("%d", &godina);

	printf("Kog meseca zelite odmor: ");
	int mesec;
	scanf("%d", &mesec);

	printf("Kog dana u mesecu zelite odmor: ");
	int dan;
	scanf("%d", &dan);

	char datumOdmora[20];
	sprintf(datumOdmora, "%d-%d-%d", godina, mesec, dan);

	dodajOdmor(sql, emailKorisnika, noteKorisnika, datumOdmora);
	odmorUBloku(sql, emailKorisnika, datumOdmora);
}

void dodajOdmor(Sql *sql, char *emailKorisnika, char *noteKorisnika, char *datumOdmora) {
	sprintf(sql->query, "insert into odmor values ('%s', '%s', '%s', 0)", datumOdmora, noteKorisnika, emailKorisnika);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}
}

void odmorUBloku(Sql *sql, char *emailKorisnika, char *datumOdmora) {
  	// ima zahtev pre/posle ovog novog dana
  	bool imaZahtevPrePosleNovogDana = false;

  	sprintf(sql->query, "select * from odmor o where o.korisnik_email = '%s' and o.deleted = 0 and (o.dan_odmora = DATE_ADD('%s', interval -1 day) or o.dan_odmora = DATE_ADD('%s', interval +1 day))", emailKorisnika, datumOdmora, datumOdmora);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}

  	sql->result = mysql_use_result(sql->connection);

  	while((sql->row = mysql_fetch_row(sql->result))) {
  		imaZahtevPrePosleNovogDana = true;
  	}

  	// ima odgovor pre/posle ovog novog dana
  	bool imaOdgovorenZahtevPrePosleNovogDana = false;

  	sprintf(sql->query, "select * from odgovorNaZahtevOdmora onzo where onzo.odmor_korisnik_email = '%s' and (onzo.status = 'odobren' or onzo.status = 'na cekanju') and (onzo.odmor_dan_odmora = DATE_ADD('%s', interval 1 day) or onzo.odmor_dan_odmora = DATE_ADD('%s', interval -1 day))", emailKorisnika, datumOdmora, datumOdmora);

  	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}
  	sql->result = mysql_use_result(sql->connection);
  	while((sql->row = mysql_fetch_row(sql->result))) {
  		imaOdgovorenZahtevPrePosleNovogDana = true;
  	}

  	if (imaOdgovorenZahtevPrePosleNovogDana == true || imaZahtevPrePosleNovogDana == true) {
  		glavniMeni(sql);
  	} else {
  		printf("Posto trazeni odmor nije u bloku odmora, da li zelite da/ne da unesete alternativni odmor: ");
  		char odgovor[3];
  		scanf("%s", odgovor);
  		if (strcmp(odgovor, "da") == 0) {
  			dodajAlternativniOdmor(sql, datumOdmora, emailKorisnika);
  		} else {
  			glavniMeni(sql);
  		}
  	}
}

void dodajAlternativniOdmor(Sql *sql, char *primarniOdmor, char *emailKorisnika) {
	printf("Unos alternativnog odmora za odmor %s:\n", primarniOdmor);
	printf("Koje godine zelite alternativni odmor: ");
	int godina;
	scanf("%d", &godina);

	printf("Kog meseca zelite alternativni odmor: ");
	int mesec;
	scanf("%d", &mesec);

	printf("Kog dana u mesecu zelite alternativni odmor: ");
	int dan;
	scanf("%d", &dan);

	char alternativniDatumOdmora[20];
	sprintf(alternativniDatumOdmora, "%d-%d-%d", godina, mesec, dan);

	dodajOdmor(sql, emailKorisnika, "alternativni", alternativniDatumOdmora);
	dodajPrimarniSekundarniOdmor(sql, emailKorisnika, primarniOdmor, alternativniDatumOdmora);
	odmorUBloku(sql, emailKorisnika, alternativniDatumOdmora);
}

void dodajPrimarniSekundarniOdmor(Sql *sql, char *emailKorisnika, char *primarniOdmor, char *alternativniOdmor){
	sprintf(sql->query, "insert into primarniSekundarniOdmor values ('%s', '%s', '%s')", emailKorisnika, primarniOdmor, alternativniOdmor);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}
}

void glavniMeni(Sql *sql) {
	printf("\n%s\n%s\n%s\n%s\n%s\n%s\n", "Unesite:", "1) Unos novih korisnika", "2) Unos novog admina", "3) Zahtev za odmor", "4) Odgovor na zahtev odmora", "5) Dodaj/oduzmi nekome dan");
	int opcija;
	scanf("%d", &opcija);
	switch(opcija) {
		case 1:
			dodajKorisnika(sql);
			break;
		case 2:
			dodajAdmina(sql);
			break;
		case 3:
			zapocniOdmor(sql);
			break;
		case 4:
			dodajOdgovorNaZahtevOdmora(sql);
			break;
		case 5:
			dodajDane(sql);
			break;
		default:
			printf("Dovidjenja!\n");
	}
}

void ispisiSveKorisnike(Sql *sql){
	sprintf (sql->query, "select * from korisnik");

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");
}

void ispisiSveAdmine(Sql *sql){
	sprintf (sql->query, "select * from admin");

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");
}

void dodajOdgovorNaZahtevOdmora(Sql *sql){
	// kom korisniku zelite da odgovorite na zahtev odmora
	printf("Od dole navedenih korisnika, izaberite email korisnika kojem zelite da odgovorite na zahtev odmora.\n");
	ispisiSveKorisnike(sql);
	printf("Izabrani korisnik: ");
	char korisnikEmail[50];
	scanf("%s", korisnikEmail);

	printf("Od dole navedenih admina sistema, izaberite email admina koji zeli da odgovori na zahtev za odmor.\n");
	ispisiSveAdmine(sql);
	printf("Izabrani admin: ");
	char adminEmail[50];
	scanf("%s", adminEmail);

	printf("Izaberite opciju za nastavak:\n");
	printf("1) Ako zelite da vidite sve zahteve za odmor\n");
	printf("2) Info - ako zelite da vidite sve odmore na koje je admin odgovorio\n");
	int opcija;
	scanf("%d", &opcija);
	switch(opcija){
		case 1:
			ispisiSveZahteveZaOdmor(sql, adminEmail, korisnikEmail);
			break;
		case 2:
			ispisSveOdgovoreneOdmore(sql, adminEmail, korisnikEmail);
			break;
		default:
			printf("Losa opcija izabrana");
	}
}

void ispisSveOdgovoreneOdmore(Sql *sql, char *adminEmail, char *korisnikEmail){
	sprintf (sql->query, "select * from odgovorNaZahtevOdmora where odmor_korisnik_email = '%s'", korisnikEmail);

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");
}

void ispisiSveZahteveZaOdmor(Sql *sql, char *adminEmail, char *korisnikEmail) {
	sprintf (sql->query, "select * from odmor where korisnik_email = '%s' and deleted = 0", korisnikEmail);

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");

  	printf("Izaberite dan odmora (u istom formatu kao sto je ispisan) za koji zelite da odgovorite: ");
  	char datumOdmora[20];
  	scanf("%s", datumOdmora);

  	odgovoriNaOdmor(sql, adminEmail, korisnikEmail, datumOdmora);
}

void odgovoriNaOdmor(Sql *sql, char *adminEmail, char *korisnikEmail, char *datumOdmora) {
	// odobren, odbijen, na cekanju
	printf("Odgovor je odobren/odbijen/na_cekanju: ");
	char status[20];
	scanf("%s", status);

	if (strcmp(status, "odbijen") != 0 && strcmp(status, "odobren") != 0 && strcmp(status, "na cekanju") != 0) {
		printf("Los status. Cao.");
		return;
	}

	printf("Poruka admina je: ");
	char temp;
	scanf("%c", &temp);
	char adminPoruka[100];
	fgets(adminPoruka, 100, stdin);

	sprintf(sql->query, "insert into odgovorNaZahtevOdmora values('%s', '%s', '%s', '%s', '%s')", status, adminPoruka, korisnikEmail, datumOdmora, adminEmail);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita odgovoriNaOdmor!\n");
    	exit (EXIT_FAILURE);
  	}

  	if (strcmp(status, "odbijen") == 0) {
  		sprintf (sql->query, "select odmor_dan_odmora1 from primarniSekundarniOdmor where odmor_dan_odmora = '%s' and odmor_korisnik_email = '%s'", datumOdmora, korisnikEmail);

	    if (mysql_query(sql->connection, sql->query)) {
	    	printf ("Neuspesno izvrsavanje upita\n");
	    	exit (EXIT_FAILURE);
	 	}

	 	sql->result = mysql_use_result(sql->connection);

	 	sql->column = mysql_fetch_fields(sql->result);

	 	char alternativniOdmor[50] = "";

	  	while ((sql->row = mysql_fetch_row(sql->result))) {
	  		strcpy(alternativniOdmor, sql->row[0]);
	  	}

	  	if (strcmp(alternativniOdmor, "") != 0) {
	  		printf("Postoji alternativni odmor na dan %s !\n", alternativniOdmor);
	  		printf("Odgovori na alternativni odmor:\n");
	  		odgovoriNaOdmor(sql, adminEmail, korisnikEmail, alternativniOdmor);
	  	} else {
	  		printf("Nema alternativnih vise.\n");
	  		glavniMeni(sql);
	  	}
  	} else {
  		ispisiLiveStatistic(sql, korisnikEmail);
  		ispisiPracenjeDana(sql, korisnikEmail);
  		glavniMeni(sql);
  	}
}

void ispisiLiveStatistic(Sql *sql, char *korisnikEmail) {
	sprintf (sql->query, "select * from aktivnaStatistikaUsera where korisnik_email = '%s'", korisnikEmail);

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");
}

void ispisiPracenjeDana(Sql *sql, char *korisnikEmail) {
	sprintf (sql->query, "select * from pracenjeDana where korisnik_email = '%s'", korisnikEmail);

    if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita\n");
    	exit (EXIT_FAILURE);
 	}

 	sql->result = mysql_use_result(sql->connection);

 	sql->column = mysql_fetch_fields(sql->result);

 	int n = mysql_field_count(sql->connection);

 	for (int i=0;i<n;i++) {
   		printf ("%s\t\t", sql->column[i].name);
 	}
  	printf ("\n\n");

  	while ((sql->row = mysql_fetch_row(sql->result))) {
    	for (int i=0;i<n;i++)
      		printf ("%s\t\t\t", sql->row[i]);
    	printf ("\n");
  	}
  	printf ("\n\n");
}

void dodajDane(Sql *sql) {
	ispisiSveKorisnike(sql);
	printf("Kojem korisniku (email) od ponudjenih zelite da dodate dane: ");
	char emailKorisnika[50];
	scanf("%s", emailKorisnika);

	printf("Koliko dana zelite da dodate: ");
	int brojDana;
	scanf("%d", &brojDana);

	printf("Za koju godinu je promena: ");
	int godina;
	scanf("%d", &godina);

	printf("Razlog promene dana: ");
	char temp;
	scanf("%c", &temp);
	char razlogPromene[100];
	fgets(razlogPromene, 100, stdin);

	sprintf(sql->query, "insert into pracenjeDana values(%d, '%s', %d, null, '%s');", godina, razlogPromene, brojDana, emailKorisnika);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}

  	printf("PRE DODAVANJA DANA: \n\n");
  	ispisiLiveStatistic(sql, emailKorisnika);

  	azurirajLiveStatistic(sql, emailKorisnika, brojDana);

  	printf("POSLE DODAVANJA DANA: \n\n");
  	ispisiLiveStatistic(sql, emailKorisnika);

}


void azurirajLiveStatistic(Sql *sql, char *emailKorisnika, int brojDana) {
	int slobodnihDana;
	sprintf(sql->query, "select preostalo_slobodnih_dana from aktivnaStatistikaUsera asu where asu.korisnik_email = '%s'", emailKorisnika);

	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}

  	sql->result = mysql_use_result(sql->connection);

  	while((sql->row = mysql_fetch_row(sql->result))) {
		slobodnihDana = atoi(sql->row[0]);
  	}

  	int noviSlobodniDani = slobodnihDana + brojDana;

	sprintf(sql->query, "update aktivnaStatistikaUsera set preostalo_slobodnih_dana = %d where korisnik_email = '%s'", noviSlobodniDani, emailKorisnika);
	if (mysql_query(sql->connection, sql->query)) {
    	printf ("Neuspesno izvrsavanje upita dodaj admina\n");
    	exit (EXIT_FAILURE);
  	}
}


