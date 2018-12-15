delimiter @
use PlanerOdmora@
-- ========================
-- 					KORISNIK
-- ========================

drop trigger if exists ai_korisnik@
create trigger ai_korisnik after insert on korisnik
for each row
begin
	insert into aktivnaStatistikaUsera values (20, 0, 0, 0, new.email);
    insert into defaultDaniPoGodini values (YEAR(CURDATE()), 20, 0, null, new.email);
end@

-- =============================
-- 			DEFAULT DANI PO GODINI
-- =============================

drop trigger if exists bi_defaultDaniPoGodini@
create trigger bi_defaultDaniPoGodini before insert on defaultDaniPoGodini
for each row
begin
	set new.ukupno = new.default_dana_godisnje + new.preneo_iz_prethodne;
end@

drop trigger if exists ai_defaultDaniPoGodini@ 
create trigger ai_defaultDaniPoGodini after insert on defaultDaniPoGodini
for each row
begin
    insert into pracenjeDana values (new.godina, 'Zaposlio se', new.default_dana_godisnje, null, new.korisnik_email);
end@

-- =======================
-- 			ZAHTEVA ODMOR
-- =======================

drop trigger if exists ai_odmor@
create trigger ai_odmor after insert on odmor
for each row
begin
	declare novi_broj_slobodnih_dana int;
    declare novi_broj_dana_na_cekanju int;
    
    declare ima_zahtev_pre_novog_dana int;
    declare ima_zahtev_posle_novog_dana int;
    
    declare ima_odgovoren_pre_novog_dana int;
    declare ima_odgovoren_posle_novog_dana int;
    
    set ima_zahtev_pre_novog_dana = (
														  select count(*) from odmor zo
                                                          where zo.korisnik_email = new.korisnik_email and
                                                          zo.deleted = 0 and
                                                          zo.dan_odmora = DATE_ADD(new.dan_odmora, interval -1 day)
														);
                                                        
	set ima_odgovoren_pre_novog_dana = (
																	select count(*) from odgovorNaZahtevOdmora onzo
                                                                    where onzo.odmor_korisnik_email = new.korisnik_email and
                                                                    (onzo.status = 'odobren' or onzo.status = 'na cekanju') and 
                                                                    onzo.odmor_dan_odmora = DATE_ADD(new.dan_odmora, interval -1 day)
																	);
                                                        
                                                        
      set ima_zahtev_posle_novog_dana = (
														  select count(*) from odmor zo
                                                          where zo.korisnik_email = new.korisnik_email and
                                                          zo.deleted = 0 and
                                                          zo.dan_odmora = DATE_ADD(new.dan_odmora, interval 1 day)
														);
                                                        
	set ima_odgovoren_posle_novog_dana = (
																		select count(*) from odgovorNaZahtevOdmora onzo
																		where onzo.odmor_korisnik_email = new.korisnik_email and
																		(onzo.status = 'odobren' or onzo.status = 'na cekanju') and 
																		onzo.odmor_dan_odmora = DATE_ADD(new.dan_odmora, interval 1 day)
																		);
                                                                        
-- 	if (ima_zahtev_pre_novog_dana > 0 or ima_odgovoren_pre_novog_dana > 0 or ima_zahtev_posle_novog_dana > 0 or ima_odgovoren_posle_novog_dana > 0)
-- 		-- Ako nije u bloku, pitaj ga da li zeli da upise novi datum kao alternativni dan odmora
-- 		-- ako zeli, upisi taj novi odmor 
-- 		then signal sqlstate '45000' set message_text = 'U BLOKU ODMORA HOCE ODMOR';
--     end if;
    

    set novi_broj_slobodnih_dana = (select preostalo_slobodnih_dana from aktivnaStatistikaUsera asu where asu.korisnik_email = new.korisnik_email) - 1;
    set novi_broj_dana_na_cekanju = (select dani_na_cekanju from aktivnaStatistikaUsera asu where asu.korisnik_email = new.korisnik_email) + 1;
    
    update aktivnaStatistikaUsera set preostalo_slobodnih_dana = novi_broj_slobodnih_dana, dani_na_cekanju = novi_broj_dana_na_cekanju where korisnik_email = new.korisnik_email;
end@

-- ==================================
-- 		 ODGOVOR NA ZAHTEV ODMORA
-- ==================================

drop trigger if exists bi_odgovorNaZahtevOdmora@
create trigger bi_odgovorNaZahtevOdmora before insert on odgovorNaZahtevOdmora
for each row
begin
	declare check_if_is_deleted tinyint;
    declare odmori_u_istom_danu int;
    declare ukupno_ljudi int;
    declare procenat_ljudi_na_odmoru float;
    
	declare novi_broj_dana_na_cekanju int;
    declare novi_broj_preostalo_slobodnih_dana int;
    
    declare da_li_je_alternativni int;
    declare status_primarnog_dana varchar(45);
    
    -- mora da bude ADMIN
	if (new.admin_korisnik_email not in (select korisnik_email from admin))
		then signal sqlstate '45000' set message_text='Moze samo admin da odobri ili odbije request za odmor.';
    end if;
    
    -- moze da odgovara samo na !delete zahteve
    set check_if_is_deleted = (select deleted from odmor zo
											   where zo.korisnik_email = new.odmor_korisnik_email and zo.dan_odmora = new.odmor_dan_odmora
											  );
    if (check_if_is_deleted = 1)
		then signal sqlstate '45000' set message_text='Ne moze da odgovori na obrisani zahtev za odmor';
    end if;
    
    -- NE SME DA ODGOVORI NA DATUM KOJI JE ALTERNATIVNI osim ako nije primarni odbijen !!!!!!!!!!! JOJ DUSANE
    set da_li_je_alternativni = (select count(*) from primarniSekundarniOdmor pso
												where pso.odmor_dan_odmora1 = new.odmor_dan_odmora
                                                );
                                                
	if (da_li_je_alternativni > 0)
    then begin
        set status_primarnog_dana = (select onzo.status from odgovorNaZahtevOdmora onzo
														where onzo.odmor_dan_odmora = (select pso.odmor_dan_odmora from primarniSekundarniOdmor pso
																												where pso.odmor_dan_odmora1 = new.odmor_dan_odmora
																												)
														);
		if (status_primarnog_dana is null or status_primarnog_dana = 'odobren' or status_primarnog_dana = 'na cekanju')
			then signal sqlstate '45000' set message_text='Ne sme da odgovori na alternativni dan ukoliko primarni nije odbijen!';
        end if;
    end;
    end if;
    
    -- ako hoce da ga odobri, proveri zauzetost tog dana
    if (new.status like 'odobren')
	then begin
    set odmori_u_istom_danu = (select count(*) from odgovorNaZahtevOdmora
												  where status like 'odobren' and 
                                                  odmor_dan_odmora = new.odmor_dan_odmora
												);
	set ukupno_ljudi = (select count(*) from korisnik);
	set procenat_ljudi_na_odmoru = (odmori_u_istom_danu + 1)*100 / ukupno_ljudi;
	if (procenat_ljudi_na_odmoru > 50)
		then begin
			set new.status = 'odbijen';
			set new.poruka_admina = 'Previse ljudi je tog dana vec na odmoru';
		end;
	end if;
    end;
    end if;
end@

drop trigger if exists ai_odgovorNaZahtevOdmora@
create trigger ai_odgovorNaZahtevOdmora after insert on odgovorNaZahtevOdmora
for each row
begin
	declare novi_broj_potvrdjeni_dani int;
    declare novi_broj_dana_na_cekanju int;
    declare novi_broj_preostalo_slobodnih_dana int;
    declare razlog_promene_dana varchar(45);
    
	if (new.status like 'odobren')
	then begin
		set razlog_promene_dana = concat('Odobren zahtev za odmor na dan: ', new.odmor_dan_odmora);
		set novi_broj_potvrdjeni_dani = (select potvrdjeni_dani from aktivnaStatistikaUsera asu where asu.korisnik_email = new.odmor_korisnik_email) + 1;
		set novi_broj_dana_na_cekanju = (select dani_na_cekanju from aktivnaStatistikaUsera asu where asu.korisnik_email = new.odmor_korisnik_email) - 1;
    
		update aktivnaStatistikaUsera set potvrdjeni_dani = novi_broj_potvrdjeni_dani, dani_na_cekanju = novi_broj_dana_na_cekanju where korisnik_email = new.odmor_korisnik_email;
        insert into pracenjeDana values (YEAR(new.odmor_dan_odmora), razlog_promene_dana, 1, null,  new.odmor_korisnik_email);
	end;
	end if;
   
	if (new.status like 'odbijen')
	then begin
		set novi_broj_preostalo_slobodnih_dana = (select preostalo_slobodnih_dana from aktivnaStatistikaUsera asu where asu.korisnik_email = new.odmor_korisnik_email) + 1;
		set novi_broj_dana_na_cekanju = (select dani_na_cekanju from aktivnaStatistikaUsera asu where asu.korisnik_email = new.odmor_korisnik_email) - 1;
    
		update aktivnaStatistikaUsera set preostalo_slobodnih_dana = novi_broj_preostalo_slobodnih_dana, dani_na_cekanju = novi_broj_dana_na_cekanju where korisnik_email = new.odmor_korisnik_email;
	end;
	end if;
   
	if (new.status like 'odobren' or new.status like 'odbijen')
	then begin
		update odmor set deleted = 1 where new.odmor_dan_odmora = dan_odmora and new.odmor_korisnik_email = korisnik_email;
	end;
	end if;
end@