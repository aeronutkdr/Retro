
-- phpMyAdmin SQL Dump
-- version 2.11.11.3
-- http://www.phpmyadmin.net
--
-- Host: 97.74.31.216
-- Generation Time: Feb 20, 2018 at 11:02 AM
-- Server version: 5.5.51
-- PHP Version: 5.1.6

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `retrosheetkdr`
--

-- --------------------------------------------------------

--
-- Table structure for table `rawevents2`
--

CREATE TABLE `rawdefenses` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `pitcher` char(8) DEFAULT NULL,
  `pitcherHand` char(1) DEFAULT NULL,
  `resPitcher` char(8) DEFAULT NULL,
  `resPitcherHand` char(1) DEFAULT NULL,
  `catcher` char(8) DEFAULT NULL,
  `firstBase` char(8) DEFAULT NULL,
  `secondBase` char(8) DEFAULT NULL,
  `thirdBase` char(8) DEFAULT NULL,
  `shortstop` char(8) DEFAULT NULL,
  `leftField` char(8) DEFAULT NULL,
  `centerField` char(8) DEFAULT NULL,
  `rightField` char(8) DEFAULT NULL,
  PRIMARY KEY (ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
