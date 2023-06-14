DROP TABLE ZAKAZNIK CASCADE CONSTRAINT;
DROP TABLE UMELEC CASCADE CONSTRAINT;
DROP TABLE AGENTURA CASCADE CONSTRAINT;
DROP TABLE DILO CASCADE CONSTRAINT;
DROP TABLE PRONAJEM CASCADE CONSTRAINT;
DROP TABLE DILO_X_PRONAJEM CASCADE CONSTRAINT;
DROP TABLE MISTNOST CASCADE CONSTRAINT;
DROP TABLE EXPOZICNI_MISTO CASCADE CONSTRAINT;
DROP TABLE VYBAVENI CASCADE CONSTRAINT;
DROP TABLE DILO_X_EXPOZICNI_MISTO CASCADE CONSTRAINT;
DROP TABLE ZAMESTNANEC CASCADE CONSTRAINT;
DROP TABLE EXPOZICNI_MISTO_X_ZAMESTNANEC CASCADE CONSTRAINT;
DROP SEQUENCE zakaznik_counter;
DROP SEQUENCE dilo_counter; 
DROP SEQUENCE pronajem_counter;
DROP SEQUENCE expozicni_misto_counter; 
DROP SEQUENCE vybaveni_counter;
DROP SEQUENCE mistnost_counter;
DROP SEQUENCE zamestnanec_counter;

--------------------------------------------------------
CREATE TABLE zakaznik
(
    id INT NOT NULL PRIMARY KEY,
    ulice VARCHAR2(255) NOT NULL,
    mesto VARCHAR2(255) NOT NULL,
    psc VARCHAR2(255) NOT NULL CHECK(LENGTH(psc) = 5),
    email VARCHAR(255) NOT NULL UNIQUE CHECK(REGEXP_LIKE (email, '^[A-Za-z]+[A-Za-z0-9.]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$')),
    telefon INT NOT NULL CHECK(REGEXP_LIKE (telefon, '^[0-9\-\+]{9,15}$'))
);

CREATE TABLE umelec
(
    id_zakaznik INT NOT NULL PRIMARY KEY,
    cislo_op VARCHAR2(255) NOT NULL UNIQUE,
    jmeno VARCHAR2(255) NOT NULL,
    prijmeni VARCHAR2(255) NOT NULL,
    CONSTRAINT fk_id_umelec FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id)
);

CREATE TABLE agentura
(
    id_zakaznik INT NOT NULL PRIMARY KEY,
    nazev_agentury VARCHAR2(255) NOT NULL,
    ic INT NOT NULL UNIQUE,
    dic VARCHAR2(255) NOT NULL UNIQUE,
    CONSTRAINT fk_id_agentura FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id)
);

CREATE TABLE dilo
(
    id_dilo INT NOT NULL PRIMARY KEY,
    nazev VARCHAR2(255) NOT NULL,
    druh VARCHAR2 (255) NOT NULL,
    autor VARCHAR2(255) NOT NULL,
    rok_vniku VARCHAR2(255) NOT NULL,
    klicove_slovo_1 VARCHAR2(255) NOT NULL,
    klicove_slovo_2 VARCHAR2(255),
    klicove_slovo_3 VARCHAR2(255),
    klicove_slovo_4 VARCHAR2(255),
    klicove_slovo_5 VARCHAR2(255),
    id_zakaznik INT NOT NULL,
    CONSTRAINT fk_id_dilo FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id)
);

CREATE TABLE pronajem
(
    id_pronajem INT NOT NULL PRIMARY KEY,
    pronajem_od DATE NOT NULL,
    pronajem_do DATE NOT NULL,
    stav_transakce VARCHAR2(255) NOT NULL,
    id_zakaznik INT NOT NULL,
    CONSTRAINT fk_id_pronajem FOREIGN KEY (id_zakaznik) REFERENCES zakaznik (id)
);

CREATE TABLE dilo_x_pronajem
(
    id_dilo INT NOT NULL,
    id_pronajem INT NOT NULL,
    CONSTRAINT pk_dilo_x_pronajem PRIMARY KEY (id_dilo, id_pronajem),
    CONSTRAINT fk_id_dilo_x_pronajem_dilo FOREIGN KEY (id_dilo) REFERENCES dilo (id_dilo),
    CONSTRAINT fk_id_dilo_x_pronajem_pronajem FOREIGN KEY (id_pronajem) REFERENCES pronajem (id_pronajem)
);

CREATE TABLE mistnost
(
    id_mistnost INT NOT NULL PRIMARY KEY,
    nazev VARCHAR2(255) NOT NULL UNIQUE,
    typ VARCHAR2(255) NOT NULL
);

CREATE TABLE expozicni_misto
(
    id_misto INT NOT NULL PRIMARY KEY,
    cena INT NOT NULL,
    velikost INT NOT NULL,
    id_pronajem INT NOT NULL,
    id_mistnost INT NOT NULL,
    CONSTRAINT fk_expozicni_misto_id_pronajem FOREIGN KEY (id_pronajem) REFERENCES pronajem (id_pronajem),
    CONSTRAINT fk_expozicni_misto_id_mistnost FOREIGN KEY (id_mistnost) REFERENCES mistnost (id_mistnost)
);

CREATE TABLE vybaveni
(
    id_vybaveni INT NOT NULL PRIMARY KEY,
    nazev VARCHAR2(255) NOT NULL,
    id_misto INT NOT NULL,
    CONSTRAINT fk_vybaveni FOREIGN KEY (id_misto) REFERENCES expozicni_misto (id_misto)
);

CREATE TABLE dilo_x_expozicni_misto
(
    id_dilo INT NOT NULL,
    id_misto INT NOT NULL,
    CONSTRAINT pk_dilo_x_expozicni_misto PRIMARY KEY (id_dilo, id_misto),
    CONSTRAINT fk_dilo_x_expozicni_misto_id_dilo FOREIGN KEY (id_dilo) REFERENCES dilo (id_dilo),
    CONSTRAINT fk_dilo_x_expozicni_misto_id_misto FOREIGN KEY (id_misto) REFERENCES expozicni_misto (id_misto)
);

CREATE TABLE zamestnanec
(
    id_zamestnanec INT NOT NULL PRIMARY KEY,
    jmeno VARCHAR2(255) NOT NULL,
    prijmeni VARCHAR2(255) NOT NULL,
    pozice VARCHAR2(255) NOT NULL,
    ulice VARCHAR2(255) NOT NULL,
    mesto VARCHAR2(255) NOT NULL,
    psc  VARCHAR2(255) NULL CHECK(LENGTH(psc) = 5),
    email VARCHAR2(255) NOT NULL UNIQUE CHECK(REGEXP_LIKE (email, '^[A-Za-z]+[A-Za-z0-9.]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$')),
    telefon INT NOT NULL UNIQUE CHECK(REGEXP_LIKE (telefon, '^[0-9\-\+]{9,15}$'))
);

CREATE TABLE expozicni_misto_x_zamestnanec
(
    id_misto INT,
    id_zamestnanec INT NOT NULL,
    CONSTRAINT pk_expozicni_misto PRIMARY KEY (id_misto, id_zamestnanec),
    CONSTRAINT fk_expozicni_misto_x_zamestnanec_id_misto FOREIGN KEY (id_misto) REFERENCES expozicni_misto (id_misto),
    CONSTRAINT fk_expozicni_misto_x_zamsestnanec_id_zamestnanec FOREIGN KEY (id_zamestnanec) REFERENCES zamestnanec (id_zamestnanec)
);


-------------------------------------------------------
CREATE SEQUENCE zakaznik_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE dilo_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE pronajem_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE expozicni_misto_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE vybaveni_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE mistnost_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE zamestnanec_counter
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 10;
----------------------------------------------

INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon) 
            VALUES (zakaznik_counter.nextval, 'Michle, Za Bumbálovskou', 'Praha 4', '14022', 'o2@o2.cz', '800020202');
INSERT INTO agentura (id_zakaznik, nazev_agentury , ic, dic) 
            VALUES (zakaznik_counter.currval, 'O2 Czech Republic a.s.', '60193336', 'CZ60193336');

INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon) 
            VALUES (zakaznik_counter.nextval, 'Palackého tøída 832/126 ', 'Brno-Královo Pole', '61200', 'test@test.cz', '600500600');
INSERT INTO umelec (cislo_op , id_zakaznik , jmeno , prijmeni) 
            VALUES ('900505/2244', zakaznik_counter.currval, 'Petr', 'Kellner');
            
INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon)
            VALUES (zakaznik_counter.nextval, 'Zaradní 420/5', 'Kuøim', '42500', 'karel120@seznam.cz', '660424505');
INSERT INTO umelec (cislo_op, id_zakaznik, jmeno, prijmeni)
            VALUES ('740820/4020',zakaznik_counter.currval, 'Karel', 'Hrubý');
            
INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon)
            VALUES (zakaznik_counter.nextval, 'Nábøežní 150/24', 'Èeské Budìjovice', '33145', 'kerol@seznam.cz', '775680870');
INSERT INTO umelec (cislo_op, id_zakaznik, jmeno, prijmeni)
            VALUES ('900115/1123', zakaznik_counter.currval, 'Karolíná', 'Nìmcová');
-----------------------------------------------

INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, klicove_slovo_4, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Madona ve skalách', 'obraz', 'Leonardo da Vinci', '1483-6','olejomalba', 'da Vinci', '15. století', 'renesance', '1');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, klicove_slovo_4, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Salvator Mundi', 'obraz', 'Leonardo da Vinci', 'pøibližnì 1500','olejomalba', 'da Vinci', '16. století', 'renesance', '1');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Venuše Mélská', 'socha', 'Alexandros of Antioch', 'desetiletí do 130 pø. n. l.', 'helénismus', 'mramor', 'Øecko', '2');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, id_zakaznik)
            VALUES (dilo_counter.nextval, 'Vìstonická venuše', 'socha', 'neznámý', '29.-25.tisíciletí pø. n. l.', 'keramika', 'paleolitické umìní', '3');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, id_zakaznik)
            VALUES (dilo_counter.nextval, 'Gutenbergova bible', 'kniha', 'Johann Gutenberg', '1454', '15. století', 'bible', '4');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, id_zakaznik)
            VALUES (dilo_counter.nextval, 'Voynichùv rukopis', 'kniha', 'neznámý', '1404–1438', '15. století', '4');

------------------------------------------------

INSERT INTO pronajem (id_pronajem, pronajem_od, pronajem_do, stav_transakce, id_zakaznik)
            VALUES (pronajem_counter.nextval, to_date('29/03/2019','DD/MM/YYYY'), to_date('29/04/2019','DD/MM/YYYY'),'zaplaceno','1');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('1', '1');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('2', '1');

INSERT INTO pronajem (id_pronajem, pronajem_od, pronajem_do, stav_transakce, id_zakaznik)
            VALUES (pronajem_counter.nextval, to_date('10/01/2019','DD/MM/YYYY'), to_date('10/04/2019','DD/MM/YYYY'),'zaplaceno','2');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('3', '2');

INSERT INTO pronajem (id_pronajem, pronajem_od, pronajem_do, stav_transakce, id_zakaznik)
            VALUES (pronajem_counter.nextval, to_date('05/02/2019','DD/MM/YYYY'), to_date('05/08/2019','DD/MM/YYYY'),'zaplaceno','3');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('4', '3');

INSERT INTO pronajem (id_pronajem, pronajem_od, pronajem_do, stav_transakce, id_zakaznik)
            VALUES (pronajem_counter.nextval, to_date('01/02/2019','DD/MM/YYYY'), to_date('01/05/2019','DD/MM/YYYY'),'zaplaceno','3');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('5', '4');
INSERT INTO dilo_x_pronajem (id_dilo, id_pronajem)
            VALUES ('6', '4');
-------------------------------------------------

INSERT INTO mistnost (id_mistnost, nazev, typ)
            VALUES (mistnost_counter.nextval, 'E100', 'klasická místnost');
INSERT INTO mistnost (id_mistnost, nazev, typ)
            VALUES (mistnost_counter.nextval, 'E101', 'klasická místnost');
INSERT INTO mistnost (id_mistnost, nazev, typ)
            VALUES (mistnost_counter.nextval, 'E102', 'krytá zahrada');
------------------------------------------------            

INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '5000', '4', '1', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'rám na obraz', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'rám na obraz', '1');        
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (1,1);
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (2,1);
        

INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
        VALUES (expozicni_misto_counter.nextval, '5000', '8', '2', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
        VALUES (vybaveni_counter.nextval, 'podstavec', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
        VALUES (vybaveni_counter.nextval, 'osvìtlená prosklená vytrína', '2');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
        VALUES (3,2);
        
INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '10000', '4', '3', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '3');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
         VALUES (vybaveni_counter.nextval, 'osvìtlená prosklená vytrína', '3');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (4,3);
        
INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '12000', '8', '4', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'osvìtlená prosklená vytrína', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'osvìtlená prosklená vytrína', '4');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (5,4);
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (6,4);
-------------------------------------------------------------------------------------

INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Jiøí', 'Poláèek', 'dozoøèí', 'Øíèní 44/180', 'Velký Beranov', '03478', 'jardapolak@seznam.cz', '665321879');
INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Radek', 'Veleba', 'dozoøèí', 'Palackého 12/6', 'Malý Beranov', '03418', 'velebny.radel@seznam.cz', '789320654');
INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Lucie', 'Hedvábná', 'recepèní', 'U Lípy 138/2', 'Podìbardy', '90582', 'luckahed@gmail.com', '602381923');
            
INSERT INTO expozicni_misto_x_zamestnanec (id_misto, id_zamestnanec)
            VALUES (1,1);
INSERT INTO expozicni_misto_x_zamestnanec (id_misto, id_zamestnanec)
            VALUES (2,1);
INSERT INTO expozicni_misto_x_zamestnanec (id_misto, id_zamestnanec)
            VALUES (1,2);
INSERT INTO expozicni_misto_x_zamestnanec (id_misto, id_zamestnanec)
            VALUES (3,2);
INSERT INTO expozicni_misto_x_zamestnanec (id_misto, id_zamestnanec)
            VALUES (4,2);
---------------------------------------------------------------------------------------

            
            
---PROJ3-------------------------------------------------------------------------------

---spojení 2 tabulek
---Výpis dìl (název díla, autora díla a druh díla), které vystavuje O2 Czech Republic a.s.
SELECT DILO.nazev, DILO.autor, DILO.druh
FROM DILO, AGENTURA
WHERE DILO.id_zakaznik = AGENTURA.id_zakaznik and AGENTURA.nazev_agentury='O2 Czech Republic a.s.' ;

--spojení 2 tabulek
--Výpis expozièních míst v místnosti E101
SELECT MISTNOST.nazev, EXPOZICNI_MISTO.id_misto, EXPOZICNI_MISTO.cena, EXPOZICNI_MISTO.velikost
FROM EXPOZICNI_MISTO, MISTNOST
WHERE MISTNOST.id_mistnost = EXPOZICNI_MISTO.id_mistnost and MISTNOST.nazev='E101';
        
---spojeni 3 tabulek
--Výpis veškerého vybabení expozièních míst v místnosti E101
SELECT VYBAVENI.id_vybaveni, VYBAVENI.nazev 
FROM VYBAVENI, EXPOZICNI_MISTO, MISTNOST
WHERE MISTNOST.nazev ='E101' and EXPOZICNI_MISTO.id_mistnost = MISTNOST.id_mistnost and VYBAVENI.id_misto = EXPOZICNI_MISTO.id_misto;

--dotazy s klauzulí GROUP BY a agregaèní funkcí
--Výpis, kolik dìl vystsavují jednotliví zákazníci.
SELECT X.id_zakaznik, COUNT(X.id_dilo)
FROM DILO X
GROUP BY X.id_zakaznik;

--Výpis, kolik expozièních míst je v jednotlivých místnostech.
SELECT X.id_mistnost, COUNT(X.id_misto)
FROM EXPOZICNI_MISTO X
GROUP BY X.id_mistnost;


--predikát EXISTS
--Výbìr všech zamìstnancù, kteøí se starají o nìjaké expozièní místo
SELECT ZAMESTNANEC.jmeno, ZAMESTNANEC.prijmeni
FROM ZAMESTNANEC
WHERE EXISTS
    (SELECT X.id_zamestnanec
    FROM EXPOZICNI_MISTO_X_ZAMESTNANEC X            
    WHERE X.id_zamestnanec = ZAMESTNANEC.id_zamestnanec);

-- predikátem IN s vnoøeným selectem
--Výpis všech zákázníku, kteøí mìli pronájem mezi lety 2018 a 2020
SELECT *
FROM ZAKAZNIK
WHERE ZAKAZNIK.id IN 
   (SELECT  PRONAJEM.id_pronajem
    FROM PRONAJEM
    WHERE PRONAJEM.pronajem_od BETWEEN '01-01-2018' and '01-01-2020'); 
