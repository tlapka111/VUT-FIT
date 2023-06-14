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
            VALUES (zakaznik_counter.nextval, 'Michle, Za Bumb�lovskou', 'Praha 4', '14022', 'o2@o2.cz', '800020202');
INSERT INTO agentura (id_zakaznik, nazev_agentury , ic, dic) 
            VALUES (zakaznik_counter.currval, 'O2 Czech Republic a.s.', '60193336', 'CZ60193336');

INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon) 
            VALUES (zakaznik_counter.nextval, 'Palack�ho t��da 832/126 ', 'Brno-Kr�lovo Pole', '61200', 'test@test.cz', '600500600');
INSERT INTO umelec (cislo_op , id_zakaznik , jmeno , prijmeni) 
            VALUES ('900505/2244', zakaznik_counter.currval, 'Petr', 'Kellner');
            
INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon)
            VALUES (zakaznik_counter.nextval, 'Zaradn� 420/5', 'Ku�im', '42500', 'karel120@seznam.cz', '660424505');
INSERT INTO umelec (cislo_op, id_zakaznik, jmeno, prijmeni)
            VALUES ('740820/4020',zakaznik_counter.currval, 'Karel', 'Hrub�');
            
INSERT INTO zakaznik (id, ulice, mesto, psc, email, telefon)
            VALUES (zakaznik_counter.nextval, 'N�b�e�n� 150/24', '�esk� Bud�jovice', '33145', 'kerol@seznam.cz', '775680870');
INSERT INTO umelec (cislo_op, id_zakaznik, jmeno, prijmeni)
            VALUES ('900115/1123', zakaznik_counter.currval, 'Karol�n�', 'N�mcov�');
-----------------------------------------------

INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, klicove_slovo_4, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Madona ve skal�ch', 'obraz', 'Leonardo da Vinci', '1483-6','olejomalba', 'da Vinci', '15. stolet�', 'renesance', '1');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, klicove_slovo_4, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Salvator Mundi', 'obraz', 'Leonardo da Vinci', 'p�ibli�n� 1500','olejomalba', 'da Vinci', '16. stolet�', 'renesance', '1');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, klicove_slovo_3, id_zakaznik) 
            VALUES (dilo_counter.nextval, 'Venu�e M�lsk�', 'socha', 'Alexandros of Antioch', 'desetilet� do 130 p�. n. l.', 'hel�nismus', 'mramor', '�ecko', '2');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, id_zakaznik)
            VALUES (dilo_counter.nextval, 'V�stonick� venu�e', 'socha', 'nezn�m�', '29.-25.tis�cilet� p�. n. l.', 'keramika', 'paleolitick� um�n�', '3');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, klicove_slovo_2, id_zakaznik)
            VALUES (dilo_counter.nextval, 'Gutenbergova bible', 'kniha', 'Johann Gutenberg', '1454', '15. stolet�', 'bible', '4');
INSERT INTO dilo (id_dilo, nazev, druh, autor, rok_vniku, klicove_slovo_1, id_zakaznik)
            VALUES (dilo_counter.nextval, 'Voynich�v rukopis', 'kniha', 'nezn�m�', '1404�1438', '15. stolet�', '4');

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
            VALUES (mistnost_counter.nextval, 'E100', 'klasick� m�stnost');
INSERT INTO mistnost (id_mistnost, nazev, typ)
            VALUES (mistnost_counter.nextval, 'E101', 'klasick� m�stnost');
INSERT INTO mistnost (id_mistnost, nazev, typ)
            VALUES (mistnost_counter.nextval, 'E102', 'kryt� zahrada');
------------------------------------------------            

INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '5000', '4', '1', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'r�m na obraz', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'r�m na obraz', '1');        
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (1,1);
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (2,1);
        

INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
        VALUES (expozicni_misto_counter.nextval, '5000', '8', '2', '1');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
        VALUES (vybaveni_counter.nextval, 'podstavec', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
        VALUES (vybaveni_counter.nextval, 'osv�tlen� prosklen� vytr�na', '2');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
        VALUES (3,2);
        
INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '10000', '4', '3', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '3');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
         VALUES (vybaveni_counter.nextval, 'osv�tlen� prosklen� vytr�na', '3');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (4,3);
        
INSERT INTO expozicni_misto (id_misto, cena, velikost, id_pronajem, id_mistnost)
            VALUES (expozicni_misto_counter.nextval, '12000', '8', '4', '2');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'podstavec', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'osv�tlen� prosklen� vytr�na', '4');
INSERT INTO vybaveni (id_vybaveni, nazev, id_misto)
            VALUES (vybaveni_counter.nextval, 'osv�tlen� prosklen� vytr�na', '4');
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (5,4);
INSERT INTO dilo_x_expozicni_misto (id_dilo, id_misto)
            VALUES (6,4);
-------------------------------------------------------------------------------------

INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Ji��', 'Pol��ek', 'dozo���', '���n� 44/180', 'Velk� Beranov', '03478', 'jardapolak@seznam.cz', '665321879');
INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Radek', 'Veleba', 'dozo���', 'Palack�ho 12/6', 'Mal� Beranov', '03418', 'velebny.radel@seznam.cz', '789320654');
INSERT INTO zamestnanec (id_zamestnanec, jmeno, prijmeni, pozice, ulice, mesto, psc, email, telefon)
            VALUES (zamestnanec_counter.nextval, 'Lucie', 'Hedv�bn�', 'recep�n�', 'U L�py 138/2', 'Pod�bardy', '90582', 'luckahed@gmail.com', '602381923');
            
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

---spojen� 2 tabulek
---V�pis d�l (n�zev d�la, autora d�la a druh d�la), kter� vystavuje O2 Czech Republic a.s.
SELECT DILO.nazev, DILO.autor, DILO.druh
FROM DILO, AGENTURA
WHERE DILO.id_zakaznik = AGENTURA.id_zakaznik and AGENTURA.nazev_agentury='O2 Czech Republic a.s.' ;

--spojen� 2 tabulek
--V�pis expozi�n�ch m�st v m�stnosti E101
SELECT MISTNOST.nazev, EXPOZICNI_MISTO.id_misto, EXPOZICNI_MISTO.cena, EXPOZICNI_MISTO.velikost
FROM EXPOZICNI_MISTO, MISTNOST
WHERE MISTNOST.id_mistnost = EXPOZICNI_MISTO.id_mistnost and MISTNOST.nazev='E101';
        
---spojeni 3 tabulek
--V�pis ve�ker�ho vybaben� expozi�n�ch m�st v m�stnosti E101
SELECT VYBAVENI.id_vybaveni, VYBAVENI.nazev 
FROM VYBAVENI, EXPOZICNI_MISTO, MISTNOST
WHERE MISTNOST.nazev ='E101' and EXPOZICNI_MISTO.id_mistnost = MISTNOST.id_mistnost and VYBAVENI.id_misto = EXPOZICNI_MISTO.id_misto;

--dotazy s klauzul� GROUP BY a agrega�n� funkc�
--V�pis, kolik d�l vystsavuj� jednotliv� z�kazn�ci.
SELECT X.id_zakaznik, COUNT(X.id_dilo)
FROM DILO X
GROUP BY X.id_zakaznik;

--V�pis, kolik expozi�n�ch m�st je v jednotliv�ch m�stnostech.
SELECT X.id_mistnost, COUNT(X.id_misto)
FROM EXPOZICNI_MISTO X
GROUP BY X.id_mistnost;


--predik�t EXISTS
--V�b�r v�ech zam�stnanc�, kte�� se staraj� o n�jak� expozi�n� m�sto
SELECT ZAMESTNANEC.jmeno, ZAMESTNANEC.prijmeni
FROM ZAMESTNANEC
WHERE EXISTS
    (SELECT X.id_zamestnanec
    FROM EXPOZICNI_MISTO_X_ZAMESTNANEC X            
    WHERE X.id_zamestnanec = ZAMESTNANEC.id_zamestnanec);

-- predik�tem IN s vno�en�m selectem
--V�pis v�ech z�k�zn�ku, kte�� m�li pron�jem mezi lety 2018 a 2020
SELECT *
FROM ZAKAZNIK
WHERE ZAKAZNIK.id IN 
   (SELECT  PRONAJEM.id_pronajem
    FROM PRONAJEM
    WHERE PRONAJEM.pronajem_od BETWEEN '01-01-2018' and '01-01-2020'); 
