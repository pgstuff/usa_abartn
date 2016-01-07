\set ECHO none
\i sql/usa_abartn.sql
\set ECHO all

CREATE TABLE usa_abartns(id serial primary key, usa_abartn usa_abartn unique);
INSERT INTO usa_abartns(usa_abartn) VALUES('001234561');
INSERT INTO usa_abartns(usa_abartn) VALUES('000000000');
INSERT INTO usa_abartns(usa_abartn) VALUES('008099990');


SELECT * FROM usa_abartns ORDER BY usa_abartn;

-- index scan
SET enable_seqscan = false;
SELECT id,usa_abartn::text FROM usa_abartns WHERE usa_abartn = '001234561';
SELECT id,usa_abartn FROM usa_abartns WHERE usa_abartn >= '001234561' LIMIT 5;
SELECT count(id) FROM usa_abartns;
SELECT count(id) FROM usa_abartns WHERE usa_abartn <> ('001234561'::text)::usa_abartn;
RESET enable_seqscan;

-- operators and conversions
SELECT '000000000'::usa_abartn < '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn > '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn < '008099990'::usa_abartn;
SELECT '000000000'::usa_abartn > '008099990'::usa_abartn;
SELECT '000000000'::usa_abartn <= '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn >= '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn <= '008099990'::usa_abartn;
SELECT '000000000'::usa_abartn >= '008099990'::usa_abartn;
SELECT '000000000'::usa_abartn <> '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn <> '008099990'::usa_abartn;
SELECT '000000000'::usa_abartn = '000000000'::usa_abartn;
SELECT '000000000'::usa_abartn = '008099990'::usa_abartn;


-- COPY FROM/TO
TRUNCATE usa_abartns;
COPY usa_abartns(usa_abartn) FROM STDIN;
000000000
008099990
\.
COPY usa_abartns TO STDOUT;

-- clean up --
DROP TABLE usa_abartns;
