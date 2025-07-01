
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

CREATE TABLE `rawevents2` (
  `gameID` char(12) DEFAULT NULL,
  `visitingTeam` char(3) DEFAULT NULL,
  `inning` tinyint(1) DEFAULT NULL,
  `battingTeam` int(4) DEFAULT NULL,
  `outs` int(4) DEFAULT NULL,
  `balls` int(4) DEFAULT NULL,
  `strikes` int(4) DEFAULT NULL,
  `pitchSequence` varchar(30) DEFAULT NULL,
  `visScore` int(4) DEFAULT NULL,
  `homeScore` int(4) DEFAULT NULL,
  `batter` char(8) DEFAULT NULL,
  `batterHand` char(1) DEFAULT NULL,
  `resBatter` char(8) DEFAULT NULL,
  `resBatterHand` char(1) DEFAULT NULL,
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
  `firstRunner` char(8) DEFAULT NULL,
  `secondRunner` char(8) DEFAULT NULL,
  `thirdRunner` char(8) DEFAULT NULL,
  `eventText` varchar(50) DEFAULT NULL,
  `leadoffFlag` char(1) DEFAULT NULL,
  `pinchhitFlag` char(1) DEFAULT NULL,
  `defensivePosition` int(4) DEFAULT NULL,
  `lineupPosition` int(4) DEFAULT NULL,
  `eventType` int(4) DEFAULT NULL,
  `batterEventFlag` char(1) DEFAULT NULL,
  `abFlag` char(1) DEFAULT NULL,
  `hitValue` int(4) DEFAULT NULL,
  `shFlag` char(1) DEFAULT NULL,
  `sfFlag` char(1) DEFAULT NULL,
  `outsOnPlay` int(4) DEFAULT NULL,
  `doublePlayFlag` char(1) DEFAULT NULL,
  `triplePlayFlag` char(1) DEFAULT NULL,
  `rbiOnPlay` int(4) DEFAULT NULL,
  `wildPitchFlag` char(1) DEFAULT NULL,
  `passedBallFlag` char(1) DEFAULT NULL,
  `fieldedBy` int(4) DEFAULT NULL,
  `battedBallType` char(1) DEFAULT NULL,
  `buntFlag` char(1) DEFAULT NULL,
  `foulFlag` char(1) DEFAULT NULL,
  `hitLocation` char(5) DEFAULT NULL,
  `numErrors` int(4) DEFAULT NULL,
  `ErrorPlayer1st` int(4) DEFAULT NULL,
  `ErrorType1st` char(1) DEFAULT NULL,
  `ErrorPlayer2nd` int(4) DEFAULT NULL,
  `ErrorType2nd` char(1) DEFAULT NULL,
  `ErrorPlayer3rd` int(4) DEFAULT NULL,
  `ErrorType3rd` char(1) DEFAULT NULL,
  `batterDest` int(4) DEFAULT NULL,
  `runnerOn1stDest` int(4) DEFAULT NULL,
  `runnerOn2ndDest` int(4) DEFAULT NULL,
  `runnerOn3rdDest` int(4) DEFAULT NULL,
  `playOnBatter` varchar(15) DEFAULT NULL,
  `playOnRunnerOn1st` varchar(15) DEFAULT NULL,
  `playOnRunnerOn2nd` varchar(15) DEFAULT NULL,
  `playOnRunnerOn3rd` varchar(15) DEFAULT NULL,
  `sbForRunnerOn1stFlag` char(1) DEFAULT NULL,
  `sbForRunnerOn2ndFlag` char(1) DEFAULT NULL,
  `sbForRunnerOn3rdFlag` char(1) DEFAULT NULL,
  `csForRunnerOn1stFlag` char(1) DEFAULT NULL,
  `csForRunnerOn2ndFlag` char(1) DEFAULT NULL,
  `csForRunnerOn3rdFlag` char(1) DEFAULT NULL,
  `poForRunnerOn1stFlag` char(1) DEFAULT NULL,
  `poForRunnerOn2ndFlag` char(1) DEFAULT NULL,
  `poForRunnerOn3rdFlag` char(1) DEFAULT NULL,
  `responsiblePitcherForRunnerOn1st` varchar(10) DEFAULT NULL,
  `responsiblePitcherForRunnerOn2nd` varchar(10) DEFAULT NULL,
  `responsiblePitcherForRunnerOn3rd` varchar(10) DEFAULT NULL,
  `newGameFlag` char(1) DEFAULT NULL,
  `endGameFlag` char(1) DEFAULT NULL,
  `pinchRunnerOn1st` char(1) DEFAULT NULL,
  `pinchRunnerOn2nd` char(1) DEFAULT NULL,
  `pinchRunnerOn3rd` char(1) DEFAULT NULL,
  `runnerRemovedForPinchRunnerOn1st` char(8) DEFAULT NULL,
  `runnerRemovedForPinchRunnerOn2nd` char(8) DEFAULT NULL,
  `runnerRemovedForPinchRunnerOn3rd` char(8) DEFAULT NULL,
  `batterRemovedForPinchHitter` char(8) DEFAULT NULL,
  `positionOfBatterRemovedForPinchHitter` int(4) DEFAULT NULL,
  `fielderWithFirstPutout` int(4) DEFAULT NULL,
  `fielderWithSecondPutout` int(4) DEFAULT NULL,
  `fielderWithThirdPutout` int(4) DEFAULT NULL,
  `fielderWithFirstAssist` int(4) DEFAULT NULL,
  `fielderWithSecondAssist` int(4) DEFAULT NULL,
  `fielderWithThirdAssist` int(4) DEFAULT NULL,
  `fielderWithFourthAssist` int(4) DEFAULT NULL,
  `fielderWithFifthAssist` int(4) DEFAULT NULL,
  `eventNum` int(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
