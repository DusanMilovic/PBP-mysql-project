use PlanerOdmora;

-- ==================
-- inserting starting values
-- ==================

-- INSERT into korisnik
insert into korisnik values
('Dusan', 'Milovic', 'dusan@gmail.com', '0645434545', 'Kraljevo', STR_TO_DATE('1995-01-12', '%Y-%m-%d'), STR_TO_DATE('2018-05-05', '%Y-%m-%d')),
('Nina', 'Senicic', 'nina@gmail.com', '0645434545', 'Kraljevo', STR_TO_DATE('1993-05-13', '%Y-%m-%d'), STR_TO_DATE('2015-04-06', '%Y-%m-%d')),
('Filip', 'Filipovic', 'filip@gmail.com', '0645434545', 'Beograd', STR_TO_DATE('1995-06-02', '%Y-%m-%d'), STR_TO_DATE('2016-03-07', '%Y-%m-%d')),
('Nikola', 'Nikolic', 'nikola@gmail.com', '0645434545', 'Novi Sad', STR_TO_DATE('1995-09-23', '%Y-%m-%d'), STR_TO_DATE('2017-02-08', '%Y-%m-%d')),
('Marko', 'Markovic', 'marko@gmail.com', '0645434545', 'Kragujevac', STR_TO_DATE('1995-11-13', '%Y-%m-%d'), STR_TO_DATE('2018-01-09', '%Y-%m-%d'));

-- insert admins
insert into admin values ('nina@gmail.com');

-- korisnik zahteva odmor
insert into odmor values
(STR_TO_DATE('2018-12-30', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'dusan@gmail.com', 0),
(STR_TO_DATE('2018-12-20', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'dusan@gmail.com', 0),
(STR_TO_DATE('2018-12-10', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'nina@gmail.com', 0),
(STR_TO_DATE('2018-12-23', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'marko@gmail.com', 0),
(STR_TO_DATE('2018-12-23', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'nikola@gmail.com', 0);

-- Testiranje da li je dan u bloku odmora tog coveka
insert into odmor values
(STR_TO_DATE('2018-12-28', '%Y-%m-%d'), 'Zelim da slavim novu godinu', 'dusan@gmail.com', 0);


-- admin odgovara na zahtev odmora
-- primer gde !admin pokusa da odgovori na zahtev odmora
insert into odgovorNaZahtevOdmora values('odobren', 'Samo napred!', 'dusan@gmail.com', STR_TO_DATE('2018-12-30', '%Y-%m-%d'), 'filip@gmail.com');

insert into odgovorNaZahtevOdmora values 
('odobren', 'Samo napred!', 'dusan@gmail.com', STR_TO_DATE('2018-12-29', '%Y-%m-%d'), 'nina@gmail.com'),
('odobren', 'Samo napred!', 'dusan@gmail.com', STR_TO_DATE('2018-12-20', '%Y-%m-%d'), 'nina@gmail.com'),
('odobren', 'Samo napred!', 'nina@gmail.com', STR_TO_DATE('2018-12-10', '%Y-%m-%d'), 'nina@gmail.com'),
-- i narednog ce prihvatiti jer gore imamo 2 razlicita coveka koji tog meseca nisu tu neki dan
('odobren', 'Samo napred!', 'marko@gmail.com', STR_TO_DATE('2018-12-23', '%Y-%m-%d'), 'nina@gmail.com'),
-- sledeci ce biti odbijen jer imamo 3 coveka u tom mesecu od 5 ljudi sto je vise od 50%
('odobren', 'Samo napred!', 'nikola@gmail.com', STR_TO_DATE('2018-12-23', '%Y-%m-%d'), 'nina@gmail.com');