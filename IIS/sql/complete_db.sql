SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS foodOrder;
DROP TABLE IF EXISTS establishment;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS food;
DROP TABLE IF EXISTS foodInOrder;
DROP TABLE IF EXISTS delivery;
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

CREATE TABLE food(
  foodId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (foodId),
  foodName VARCHAR (255) NOT NULL,
  foodDescription TEXT,
  foodType VARCHAR(255),
  foodWeight INT,
  foodPrice INT NOT NULL,
  establishmentId INT NOT NULL,
  foodAttributes SET('Smažené','RAW','Bio','Grilované','Bezmasé') CHARACTER SET utf8 COLLATE utf8_czech_ci NULL,
  foodImageMime VARCHAR(50) NULL,
  foodImageData MEDIUMBLOB NULL
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
  price INT NOT NULL,
  deliveryId INT DEFAULT NULL
);

CREATE TABLE foodInOrder(
  foodInOrderId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (foodInOrderId),
  foodCount INT NOT NULL,
  foodInOrder_foodId INT NOT NULL,
  foodInOrder_foodOrderId INT NOT NULL
);

CREATE TABLE delivery(
  deliveryId INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (deliveryId),
  driverId INT DEFAULT NULL,
  onway VARCHAR(6) NOT NULL DEFAULT 'false',
  deliverySolved VARCHAR(6) NOT NULL DEFAULT 'false',
  departsAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


ALTER TABLE foodOrder
ADD FOREIGN KEY (userId) REFERENCES users(userId) ON DELETE CASCADE;

ALTER TABLE food
ADD FOREIGN KEY (establishmentId) REFERENCES establishment(establishmentId) ON DELETE CASCADE;

ALTER TABLE foodInOrder
ADD FOREIGN KEY (foodInOrder_foodId) REFERENCES food(foodId) ON DELETE CASCADE;
ALTER TABLE foodInOrder
ADD FOREIGN KEY (foodInOrder_foodOrderId) REFERENCES foodOrder(foodOrderId) ON DELETE CASCADE;

ALTER TABLE delivery
ADD FOREIGN KEY (driverId) REFERENCES users(userId) ON DELETE CASCADE;

ALTER TABLE foodOrder
ADD FOREIGN KEY (deliveryId) REFERENCES delivery(deliveryId) ON DELETE CASCADE;


INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('admin','admin@admin','+420773220097','Martin','Vlach','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Zahradní','26','2019-11-17');
INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('driver','driver@driver','+420605532136','Lukáš','Hekrdla','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Bezručova','51','2019-11-17');
INSERT INTO `users`(`roleName`,`email`, `phoneNumber`, `firstName`, `lastName`, `password`, `deliveryCity`, `deliveryStreet`, `deliveryHouseNumber`, `registrationDate`) VALUES ('operator','operator@operator','+420773330111','Tomáš','Tlapák','6a284155906c26cbca20c53376bc63ac','Velké Meziříčí','Křižní','21','2019-11-17');
