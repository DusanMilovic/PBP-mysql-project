# PBP-mysql-project

## Zadatak: Projekat predstavlja uproscen proces pracenja slobodnih dana korisnika u okviru jedne firme.
**Korisnika** definisu sledeci atributi: ime, prezime, email, broj_telefona, adresa, datum_rodjenja, datum_zaposljavanja.
Korisnik moze da bude **admin** sistema, a i ne mora.

U bazi se cuvaju za svakog korisnika informacije o **trenutnom stanju njegovih slobodnih dana**. Cuvaju se: preostalo_slobodnih_dana, potvrdjeni_dani (dani koji su odobreni od strane admina u buducnosti), dani_na_cekanju (dani za koje admin nije odgovorio jos uvek), potroseni_dani (Ukoliko budem koristio cron, imace svrhu) (dani koji su potvrdjeni od strane admina i prosli su danasnji datum).

U bazi takodje se cuvaju za svakog korisnika podaci o njegovim **default danima** koje dobija svake godine (na dan zaposlenja ili svakog prvog januara, ponovo cron?). Tu cuvamo: godina, default_dana_godisnje, preneo_iz_prethodne_godine, ukupno (svi podaci iz trackera sabrani).

(Tracker) Za svaku eksplicitnu (kada se zaposli dobija dane, ili nekom zaslugom dobije ili mu se oduzmu dani) promenu slobodnih dana, bila pozitivna da je dobio dane ili negativna da je potrosio dane, pamtimo **log** u bazi. Od informacija tu treba da imamo: godina, razlog_menjanja_dana, broj_dana_promena.

Korisnik moze da **zahteva odmor** gde navodi poruku zasto zeli odmor, dan_odmora (datum na koji dan zeli odmor).
Jedino admin sme i moze da **odgovori na zahtev odmora**. Moze da ga prihvati odbije ili ostavi na cekanju. Od podataka baza pamti: status ('odbijen', 'prihvacen', 'na cekanju'), poruka admina (obrazlozenje admina za njegov odgovor).
Kada je status odbijen ili prihvacen, brise se taj zahtev odmora. (Necemo zaista brisati, vec cemo imati flag koji nam  ukazuje da li je 'izbrisan' ili ne, kako bismo u bazi uvek imali sacuvano sve zahteve korisnika).

U firmi nikada ne sme da se desi da vise od 50% zaposlenih je u istom danu na odmoru.

Ukoliko se dani nekog korisnika ne nalaze u bloku (tacnije ako imamo vise od jednog dana zaredom), onda za taj dan korisnik moze da navede alternativni dan odmora koji ce biti razmatran u slucaju da primarni dan bude odbijen od strane admina. Jedan korisnik ne moze da ima isti alternativni odmor za razlicite odmore.

## Uslovi projekta:
* Rekurzivni odnos
  * zahtevaOdmor
* Slab entitet ili odnos specijalizacija/generalizacija
  * admin
* Trigeri kojima se menja stanje baze
  * Prilikom kreiranja korisnika automatsi se dodeljuju default dani tom korisniku i aktivna statistika korisnika
  * Prilikom ubacivanja u default dane korisnika azurirati tabelu sa logovima
  
