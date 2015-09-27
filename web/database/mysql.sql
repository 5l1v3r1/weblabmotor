CREATE SCHEMA `cer` DEFAULT CHARACTER SET utf8 ;

CREATE TABLE `cer`.`usrPermanente` (
  `tempo` TIMESTAMP NOT NULL,
  `a0` INT NULL,
  `a1` INT NULL,
  `a2` INT NULL,
  `a3` INT NULL,
  `a4` INT NULL,
  `a5` INT NULL,
  PRIMARY KEY (`tempo`));

CREATE TABLE `cer`.`usrTemporario` (
  `tempo` TIMESTAMP NOT NULL,
  `a0` INT NULL,
  `a1` INT NULL,
  `a2` INT NULL,
  `a3` INT NULL,
  `a4` INT NULL,
  `a5` INT NULL,
  PRIMARY KEY (`tempo`));

CREATE TABLE `cer.`ucr` (
  `tempo` TIMESTAMP NOT NULL,
  `d0` INT NULL,
  `d1` INT NULL,
  `d2` INT NULL,
  `d3` INT NULL,
  `d4` INT NULL,
  `d5` INT NULL,
  `d6` INT NULL,
  `d7` INT NULL,
  `d8` INT NULL,
  `d9` INT NULL,
  `d10` INT NULL,
  `d11` INT NULL,
  `d12` INT NULL,
  `d13` INT NULL,
  PRIMARY KEY (`tempo`));
