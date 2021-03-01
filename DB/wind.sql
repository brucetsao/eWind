-- phpMyAdmin SQL Dump
-- version 4.8.2
-- https://www.phpmyadmin.net/
--
-- 主機: localhost
-- 產生時間： 2021 年 01 月 04 日 03:42
-- 伺服器版本: 5.5.57-MariaDB
-- PHP 版本： 5.6.31

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 資料庫： `ncnuiot`
--

-- --------------------------------------------------------

--
-- 資料表結構 `wind`
--

CREATE TABLE `wind` (
  `id` int(11) NOT NULL,
  `sysdatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `speed` float NOT NULL COMMENT '風速',
  `way` int(12) NOT NULL COMMENT '風向',
  `temp` float NOT NULL COMMENT '溫度',
  `humid` float NOT NULL COMMENT '濕度',
  `ip` varchar(20) NOT NULL COMMENT '連上IP位址',
  `DT` varchar(12) NOT NULL COMMENT 'YYYYMMDDHHMM',
  `mac` varchar(16) DEFAULT NULL COMMENT '連上網卡號碼'
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='吳厝國小樹屋風速資訊表';

--
-- 已匯出資料表的索引
--

--
-- 資料表索引 `wind`
--
ALTER TABLE `wind`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `ckduplicate` (`mac`,`DT`),
  ADD KEY `dt` (`DT`);

--
-- 在匯出的資料表使用 AUTO_INCREMENT
--

--
-- 使用資料表 AUTO_INCREMENT `wind`
--
ALTER TABLE `wind`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
