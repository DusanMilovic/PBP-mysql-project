drop database if exists PlanerOdmora;
create database PlanerOdmora;
use PlanerOdmora;

CREATE TABLE IF NOT EXISTS korisnik (
  ime VARCHAR(45) NULL,
  prezime VARCHAR(45) NULL,
  email VARCHAR(45) PRIMARY KEY NOT NULL,
  broj_telefona VARCHAR(45) NULL,
  adresa VARCHAR(45) NULL,
  datum_rodjenja DATE NULL,
  datum_zaposljavanja DATE NULL
);

CREATE TABLE IF NOT EXISTS aktivnaStatistikaUsera (
  preostalo_slobodnih_dana INT NULL DEFAULT 0,
  potvrdjeni_dani INT NULL DEFAULT 0,
  dani_na_cekanju INT NULL DEFAULT 0,
  potroseni_dani INT NULL DEFAULT 0,
  korisnik_email VARCHAR(45) PRIMARY KEY NOT NULL,
  CONSTRAINT fk_aktivnaStatistikaUsera_korisnik
    FOREIGN KEY (korisnik_email)
    REFERENCES korisnik (email)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS defaultDaniPoGodini (
  godina INT NOT NULL,
  default_dana_godisnje INT NULL,
  preneo_iz_prethodne INT NULL,
  ukupno INT NULL,
  korisnik_email VARCHAR(45) NOT NULL,
  PRIMARY KEY (godina, korisnik_email),
  CONSTRAINT fk_defaultDaniPoGodini_korisnik1
    FOREIGN KEY (korisnik_email)
    REFERENCES korisnik (email)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS admin (
  korisnik_email VARCHAR(45) NOT NULL,
  PRIMARY KEY (korisnik_email),
  CONSTRAINT fk_admin_korisnik1
    FOREIGN KEY (korisnik_email)
    REFERENCES korisnik (email)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS pracenjeDana (
  godina INT NULL,
  razlog_menjanja_dana VARCHAR(45) NULL,
  broj_dana_promena INT NULL,
  id INT NOT NULL AUTO_INCREMENT,
  korisnik_email VARCHAR(45) NOT NULL,
  PRIMARY KEY (id),
  constraint fk_pracenjeDana_korisnik foreign key (korisnik_email) references korisnik (email)
  on delete no action
  on update no action
);

CREATE TABLE IF NOT EXISTS odmor (
  dan_odmora DATE NOT NULL,
  poruka_korisnika VARCHAR(45) NULL,
  korisnik_email VARCHAR(45) NOT NULL,
  deleted TINYINT NULL DEFAULT 0,
  PRIMARY KEY (korisnik_email, dan_odmora),
  CONSTRAINT fk_zahtevaOdmor_korisnik1
    FOREIGN KEY (korisnik_email)
    REFERENCES korisnik (email)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS primarniSekundarniOdmor (
  odmor_korisnik_email VARCHAR(45) NOT NULL,
  odmor_dan_odmora DATE NOT NULL,
  odmor_dan_odmora1 DATE NULL,
  PRIMARY KEY (odmor_korisnik_email, odmor_dan_odmora),
  INDEX fk_primarniSekundarniOdmor_odmor2_idx (odmor_dan_odmora1 ASC),
  CONSTRAINT fk_primarniSekundarniOdmor_odmor1
    FOREIGN KEY (odmor_korisnik_email , odmor_dan_odmora)
    REFERENCES odmor (korisnik_email , dan_odmora)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT fk_primarniSekundarniOdmor_odmor2
    FOREIGN KEY (odmor_dan_odmora1)
    REFERENCES odmor (dan_odmora)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS odgovorNaZahtevOdmora (
  status VARCHAR(45) NULL,
  poruka_admina VARCHAR(45) NULL,
  odmor_korisnik_email VARCHAR(45) NOT NULL,
  odmor_dan_odmora DATE NOT NULL,
  admin_korisnik_email VARCHAR(45) NOT NULL,
  PRIMARY KEY (odmor_korisnik_email, odmor_dan_odmora),
  CONSTRAINT fk_odgovorNaZahtevOdmora_zahtevaOdmor1
    FOREIGN KEY (odmor_korisnik_email , odmor_dan_odmora)
    REFERENCES odmor (korisnik_email , dan_odmora)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT fk_odgovorNaZahtevOdmora_admin1
    FOREIGN KEY (admin_korisnik_email)
    REFERENCES admin (korisnik_email)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);