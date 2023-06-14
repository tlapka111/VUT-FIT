
SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS establishmentOffer;
DROP TABLE IF EXISTS foodOrder;
DROP TABLE IF EXISTS establishment;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS food;
DROP TABLE IF EXISTS foodInOrder;
SET FOREIGN_KEY_CHECKS = 1;


CREATE TABLE users(
  userId INT NOT NULL AUTO_INCREMENT,
  roleName ENUM ('admin','operator','driver', 'user') NOT NULL DEFAULT 'user',
  PRIMARY KEY(userId),
  email VARCHAR(50) NOT NULL,
  phoneNumber VARCHAR(15) NULL,
  firstName VARCHAR(50) NOT NULL,
  lastName VARCHAR(50) NOT NULL,
  password VARCHAR(100) NOT NULL,
  deliveryCity VARCHAR(100) DEFAULT NULL,
  deliveryStreet VARCHAR(100) DEFAULT NULL,
  deliveryHouseNumber VARCHAR(100) DEFAULT NULL,
  registrationDate DATE DEFAULT NULL,
  isOpen VARCHAR(6) NOT NULL DEFAULT 'true'
);


CREATE TABLE establishment(
  establishmentId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (establishmentId),
  name VARCHAR(255) NOT NULL,
  description TEXT DEFAULT NULL,
  priceCategory INT DEFAULT NULL,
  city VARCHAR(100) NOT NULL,
  street VARCHAR(100) NOT NULL,
  houseNumber VARCHAR(100) NOT NULL,
  isOpen VARCHAR(6) NOT NULL DEFAULT 'true'
);

CREATE TABLE establishmentOffer(
  establishmentOfferId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (establishmentOfferId),
  establishmentId INT NOT NULL,
  offerDate DATE NOT NULL,
  description TEXT DEFAULT NULL
);


CREATE TABLE food(
  foodId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (foodId),
  foodName VARCHAR (255) NOT NULL,
  foodDescription TEXT,
  foodType VARCHAR(255),
  foodWeight INT,
  foodPrice INT NOT NULL,
  establishmentId INT NOT NULL,
  foodImage VARCHAR (100) NOT NULL DEFAULT 'bezObrazku.jpg',
  foodAttributes SET('Smažené','RAW','Bio','Grilované','Bezmasé') CHARACTER SET utf8 COLLATE utf8_czech_ci NULL
);


CREATE TABLE foodOrder(
  foodOrderId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (FoodOrderId),
  createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  orderState ENUM ('new','inprogress','onway','solved') NOT NULL DEFAULT 'new',
  solved VARCHAR(6) NOT NULL DEFAULT 'false',
  userId INT DEFAULT NULL,
  firstName VARCHAR(100) NOT NULL,
  lastName VARCHAR(100) NOT NULL,
  phoneNumber VARCHAR(100) NOT NULL,
  deliveryCity VARCHAR(100) NOT NULL,
  deliveryStreet VARCHAR(100) NOT NULL,
  deliveryHouseNumber VARCHAR(100) NOT NULL,
  price INT NOT NULL
);

CREATE TABLE foodInOrder(
  foodInOrderId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (foodInOrderId),
  foodCount INT NOT NULL,
  foodInOrder_foodId INT NOT NULL,
  foodInOrder_foodOrderId INT NOT NULL
);

ALTER TABLE foodOrder
ADD FOREIGN KEY (userId) REFERENCES users(userId) ON DELETE CASCADE;

ALTER TABLE food
ADD FOREIGN KEY (establishmentId) REFERENCES establishment(establishmentId) ON DELETE CASCADE;

ALTER TABLE foodInOrder
ADD FOREIGN KEY (foodInOrder_foodId) REFERENCES food(foodId) ON DELETE CASCADE;
ALTER TABLE foodInOrder
ADD FOREIGN KEY (foodInOrder_foodOrderId) REFERENCES foodOrder(foodOrderId) ON DELETE CASCADE;


INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('admin','admin@admin','+420773220097','Martin','Vlach','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Zahradní','26','2019-11-17');
INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('driver','driver@driver','+420773220097','Martin','Vlach','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Zahradní','26','2019-11-17');
INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('operator','operator@operator','+420773220097','Martin','Vlach','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Zahradní','26','2019-11-17');

INSERT INTO `establishment`(`name`, `description`, `priceCategory`, `city`, `street`, `houseNumber`) 
       VALUES ('U Bílého koníčka','Stravování zajistíme v příjemném prostředí restaurace nebo v nově zřízené pizzerii v přízemí.','3','Velké Meziříčí','Hornoměstská','348/5');

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Smažená vepřová kotleta bramborový salát s maj.', 'Denni', 120, 125, 1, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Moravský vrabec, špenát, bramborové knedlíky', 'Denni', 150, 124, 1);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId)
       VALUES ('Vídeňský guláš, houskové knedlíky', 'Denni', 120, 124, 1);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId)
       VALUES ('Svíčková na smetaně, kynuté houskové knedlíky', 'Denni', 120, 147, 1);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Uzená vepřová plec a pečená vepřová krkovice, hlávkové a kysané zelí, bramborové knedlíky a bramboráčky', 'Denni', 150, 143, 1);

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Ďábelský biftek servírovaný s pikantní chilli omáčkou a hřebínkem anglické slaniny', 'Stala', 200, 355, 1, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Šťavnatý hovězí pfeffersteak s čerstvě mletým pepřem a mořskou solí', 'Stala', 200, 355, 1, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId,  foodAttributes) 
       VALUES ('Tatarský biftek -8ks topinek', 'Stala', 200, 355, 1, 'RAW');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId,  foodAttributes) 
       VALUES ('Grilovaný steak z kuřecích prsíček s dijonským přelivem', 'Stala', 200, 179, 1, 'Grilované');

INSERT INTO Food(foodName, foodType, foodWeight ,foodPrice, establishmentId) 
       VALUES ('Svařené víno bílé - Irsai Oliver', 'Napoj', 200, 43, 1);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('12° Pilsner Urguell', 'Napoj', 500, 41, 1);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Mattoni – perlivá', 'Napoj', 330, 25, 1);







INSERT INTO `establishment`(`name`, `priceCategory`, `city`, `street`, `houseNumber`) 
       VALUES ('Acapulco restaurant','2','Vyškov','Sušilova','38/8');

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Rizoto s kuřecím masem, rajčatovým pestem a pažitkou', 'Stala', 250, 179, 2);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Penne s grilovaným norským lososem a špenátem', 'Stala', 300, 189, 2, 'Grilované');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Marinovaný losos v růžovém pepři s kaparovými brambůrky', 'Stala', 300, 189, 2);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Smažené medailonky z panenky', 'Stala', 150, 179, 2, 'Samžené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Salát s grilovaným kozím sýrem, dýňovými semínky a červenou řepou', 'Stala', 300, 199, 2, 'Grilované');

INSERT INTO Food(foodName, foodDescription  ,foodType, foodPrice, establishmentId) 
       VALUES ('Prosecco Asolo superiore „Fano“ Astoria', 'Vinařství Astoria Itálie, extra brut', 'Napoj', 399, 2);
INSERT INTO Food(foodName, foodDescription  ,foodType, foodPrice, establishmentId) 
       VALUES ('Prosecco', 'Vinařství Sant Anna Itálie, frizzante', 'Napoj', 289, 2);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Bernard 10°', 'Napoj', 500, 37, 2);








INSERT INTO `establishment`(`name`, `priceCategory`, `city`, `street`, `houseNumber`) 
       VALUES ('Restaurace BORGO AGNESE','5','Brno','Kopečná','43');

INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Telecí steak, uzený jogurt, celer 5-ti vůní, patison, cuketa, tonka fazole, marsala', 'Stala', 549, 3);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Srnčí hřbet, topinambur, povidla, řapíkatý celer', 'Stala', 599, 3);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Pečený bok z divočáka, tvarůžky, škvarky, ratte brambory', 'Stala', 449, 3);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Foie gras, kakao, černý bez, vlašský ořech', 'Stala', 299, 3);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Kachní prso, kysané zelí, variace knedlíků', 'Stala', 399, 3);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Mandlová panna cotta, mango, melasa, pomeranč', 'Stala', 179, 3);

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Hennessy Imperial Borgo Agnese', 'Napoj', 40, 1999, 3);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Richard Hennessy', 'Napoj', 20, 3399, 3);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Créme de Cassis', 'Napoj', 40, 85, 3);








INSERT INTO `establishment`(`name`, `priceCategory`, `city`, `street`, `houseNumber`) 
       VALUES ('Gril & Club na Jakubáku','3','Brno','Běhounská','18');

INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Kachní prso, dýňové pyré, vinná omáčka', 'Stala', 180, 269, 4);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Slávky na bílém víně a zelenině, pečivo', 'Stala', 400, 259, 4);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Vepřová panenka, bruselské kapustičky a jablko', 'Stala', 180, 239, 4);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES (' Burger z vyzrálého hovězího z Mikrofarmy, čedar, slanina, BBQ omáčka, salát Coleslaw v máslové briošce', 'Stala', 150, 189, 4);

INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Cibulačka + Hovězí hrudí na pepři, bulgur', 'Denni', 120, 4);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Carpaccio z červené řepy, kozí sýr, rukola', 'Denni', 90, 4);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Rajčatová polévka s bazalkou + Kuřecí steak, salsa verde, bramborové pyré', 'Denni', 140, 4);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Cibulačka + Smažený hermelín s poličanem, máslové brambory', 'Denni', 135, 4);

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Patrón Reposado', 'Napoj', 40, 165, 4);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Březňák 11°', 'Napoj', 500, 40, 4);
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId) 
       VALUES ('Coca Cola, Coca Cola Zero', 'Napoj', 200, 45, 4);
INSERT INTO Food(foodName, foodType, foodPrice, establishmentId) 
       VALUES ('Latte macchiato', 'Napoj', 60, 4);







INSERT INTO `establishment`(`name`, `priceCategory`, `city`, `street`, `houseNumber`) 
       VALUES ('La Bašta Břeclav','1','Břeclav','Lidická','1');

INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Stripsy s hranolky a oblohou','Stala', 350, 139, 5, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Smažený sýr s tatarkou','Stala', 100, 109, 5, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Smažený hermelín','Stala', 100, 109, 5, 'Smažené');
INSERT INTO Food(foodName, foodType, foodWeight, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Kuřecí řízek','Stala', 150, 119, 5, 'Smažené');
INSERT INTO Food(foodName, foodDescription, foodType, foodPrice, establishmentId, foodAttributes) 
       VALUES ('Burger piccante', '200g  hovězího masa, čedar, grilovaná ventricina, jalapeňos, rajče, polníček, omáčka sweet-chilly','Stala', 139, 5, 'Grilované');
INSERT INTO Food(foodName, foodDescription, foodType, foodPrice, establishmentId) 
       VALUES ('Cheeseburger', '200g  hovězího masa, 2x čedar, slanina, cibulové chutney, nakládaný okurek, salát, kečup, hořčice','Stala', 139, 5);
INSERT INTO Food(foodName, foodDescription, foodType, foodPrice, establishmentId) 
       VALUES ('Messina', 'smetanový základ, mozzarella, uzený sýr, cibule, petrželka, šunka, feferonky','Stala', 154, 5);

INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Coca-Cola', 'Napoj',  1500, 49, 5);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Fanta', 'Napoj',  1500, 49, 5);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Sprite', 'Napoj',  1500, 49, 5);
INSERT INTO Food(foodName, foodType, foodWeight,foodPrice, establishmentId) 
       VALUES ('Starobrno', 'Napoj',  500, 29, 5);