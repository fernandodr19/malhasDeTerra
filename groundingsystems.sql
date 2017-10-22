DROP SCHEMA groundingsystems;
CREATE SCHEMA groundingsystems;
USE groundingsystems;

CREATE TABLE `Users` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`email` varchar(30) NOT NULL UNIQUE,
	`password` varchar(30) NOT NULL,
	`firstName` varchar(30) NOT NULL,
	`lastName` varchar(30) NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `Projects` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` varchar(30) NOT NULL,
	`substation` varchar(30) NOT NULL,
	`description` varchar(300),
	`designer` varchar(30),
	`date` DATE,
	`lastGsId` INT,
	PRIMARY KEY (`id`)
);

CREATE TABLE `GroundingSystems` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`projectId` INT NOT NULL,
	`name` varchar(30) NOT NULL,
	`conductorsMaxLength` FLOAT,
	`nLayers` FLOAT DEFAULT '1',
	`firstLayerDepth` FLOAT,
	`firstLayerResistivity` FLOAT,
	`secondLayerResistivity` FLOAT,
	`crushedStoneLayerDepth` FLOAT,
	`crushedStoneLayerResistivity` FLOAT,
	`injectedCurrent` FLOAT,
	PRIMARY KEY (`id`)
);

CREATE TABLE `Conductors` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`gsId` INT NOT NULL,
	`x1` FLOAT NOT NULL,
	`y1` FLOAT NOT NULL,
	`z1` FLOAT NOT NULL,
	`x2` FLOAT NOT NULL,
	`y2` FLOAT NOT NULL,
	`z2` FLOAT NOT NULL,
	`cableId` INT NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `Cables` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`code` varchar(30) NOT NULL,
	`diameter` FLOAT NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `Points` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`gsId` INT NOT NULL,
	`x` FLOAT NOT NULL,
	`y` FLOAT NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `Profiles` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`gsId` INT NOT NULL,
	`x1` FLOAT NOT NULL,
	`y1` FLOAT NOT NULL,
	`x2` FLOAT NOT NULL,
	`y2` FLOAT NOT NULL,
	`precision` FLOAT NOT NULL,
	`touch` BOOLEAN NOT NULL DEFAULT TRUE,
	`step` BOOLEAN NOT NULL DEFAULT TRUE,
	PRIMARY KEY (`id`)
);

CREATE TABLE `UserProjects` (
	`userId` INT NOT NULL,
	`projectId` INT NOT NULL,
	`owner` BOOLEAN NOT NULL DEFAULT TRUE,
	`readOnly` BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY (`userId`,`projectId`)
);

ALTER TABLE `Projects` ADD CONSTRAINT `Projects_fk0` FOREIGN KEY (`lastGsId`) REFERENCES `GroundingSystems`(`id`);

ALTER TABLE `GroundingSystems` ADD CONSTRAINT `GroundingSystems_fk0` FOREIGN KEY (`projectId`) REFERENCES `Projects`(`id`);

ALTER TABLE `Conductors` ADD CONSTRAINT `Conductors_fk0` FOREIGN KEY (`gsId`) REFERENCES `GroundingSystems`(`id`);

ALTER TABLE `Conductors` ADD CONSTRAINT `Conductors_fk1` FOREIGN KEY (`cableId`) REFERENCES `Cables`(`id`);

ALTER TABLE `Points` ADD CONSTRAINT `Points_fk0` FOREIGN KEY (`gsId`) REFERENCES `GroundingSystems`(`id`);

ALTER TABLE `Profiles` ADD CONSTRAINT `Profiles_fk0` FOREIGN KEY (`gsId`) REFERENCES `GroundingSystems`(`id`);

ALTER TABLE `UserProjects` ADD CONSTRAINT `UserProjects_fk0` FOREIGN KEY (`userId`) REFERENCES `Users`(`id`);

ALTER TABLE `UserProjects` ADD CONSTRAINT `UserProjects_fk1` FOREIGN KEY (`projectId`) REFERENCES `Projects`(`id`);

INSERT INTO `Users` VALUES (1,'fdr','123','Fernando','Da Rós Filho');
INSERT INTO `Projects` VALUES (1,'Projeto 1','SE 1','Projeto teste','Fluxo',NULL, NULL);
INSERT INTO `UserProjects` VALUES (1,1,1,0);
INSERT INTO `Cables` VALUES (1,'DOTTEREL',0.5),(2,'AÇO 3/8\"',0.5);
INSERT INTO `GroundingSystems` VALUES (1,1,'Malha 1',0.5,1,0,60,1000,0.15,3000,9170);
INSERT INTO `Conductors` VALUES (1,1,0,0,-0.5,0,14.3,-0.5,1),(2,1,1,0,-0.5,1,14.3,-0.5,1),(3,1,2,0,-0.5,2,14.3,-0.5,1),(4,1,3.8,0,-0.5,3.8,14.3,-0.5,1),(5,1,6.4,0,-0.5,6.4,14.3,-0.5,1),(6,1,9.1,0,-0.5,9.1,14.3,-0.5,1),(7,1,11.8,0,-0.5,11.8,14.3,-0.5,1),(8,1,14.4,0,-0.5,14.4,14.3,-0.5,1),(9,1,16.4,0,-0.5,16.4,14.3,-0.5,1),(10,1,17.4,0,-0.5,17.4,14.3,-0.5,1),(11,1,18.4,0,-0.5,18.4,14.3,-0.5,1),(12,1,0,0,-0.5,18.4,0,-0.5,1),(13,1,0,1,-0.5,18.4,1,-0.5,1),(14,1,0,2,-0.5,18.4,2,-0.5,1),(15,1,0,4,-0.5,18.4,4,-0.5,1),(16,1,0,6.2,-0.5,18.4,6.2,-0.5,1),(17,1,0,8.4,-0.5,18.4,8.4,-0.5,1),(18,1,0,10.6,-0.5,18.4,10.6,-0.5,1),(19,1,0,12.3,-0.5,18.4,12.3,-0.5,1),(20,1,0,13.3,-0.5,18.4,13.3,-0.5,1),(21,1,0,14.3,-0.5,18.4,14.3,-0.5,1),(22,1,0,0,-0.5,0,0,-3.5,1),(23,1,18.4,0,-0.5,18.4,0,-3.5,1),(24,1,0,14.3,-0.5,0,14.3,-3.5,1),(25,1,18.4,14.3,-0.5,18.4,14.3,-3.5,1);
INSERT INTO `Points` VALUES (1,1,5,1);
INSERT INTO `Profiles` VALUES (1,1,1,-1,1,15.3,0.1,1,1);