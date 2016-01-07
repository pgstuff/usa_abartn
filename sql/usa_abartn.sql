/*
 * Author: The maintainer's name
 * Created at: Wed Oct 14 23:12:59 -0400 2015
 *
 */

SET client_min_messages = warning;

-- SQL definitions
CREATE TYPE usa_abartn;

-- basic i/o functions
CREATE OR REPLACE FUNCTION usa_abartn_in(cstring) RETURNS usa_abartn AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_out(usa_abartn) RETURNS cstring AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_send(usa_abartn) RETURNS bytea AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_recv(internal) RETURNS usa_abartn AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE usa_abartn (
	input = usa_abartn_in,
	output = usa_abartn_out,
	send = usa_abartn_send,
	receive = usa_abartn_recv,
	internallength = 4,
	passedbyvalue
);

-- functions to support btree opclass
CREATE OR REPLACE FUNCTION usa_abartn_lt(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_le(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_eq(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_ne(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_ge(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_gt(usa_abartn, usa_abartn) RETURNS bool AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION usa_abartn_cmp(usa_abartn, usa_abartn) RETURNS int4 AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;

-- to/from text conversion
CREATE OR REPLACE FUNCTION usa_abartn_to_text(usa_abartn) RETURNS text AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION text_to_usa_abartn(text) RETURNS usa_abartn AS '$libdir/usa_abartn'
LANGUAGE C IMMUTABLE STRICT;

-- operators
CREATE OPERATOR < (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_lt,
	commutator = >, negator = >=,
	restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_le,
	commutator = >=, negator = >,
	restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_eq,
	commutator = =, negator = <>,
	restrict = eqsel, join = eqjoinsel,
	merges
);
CREATE OPERATOR <> (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_ne,
	commutator = <>, negator = =,
	restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR > (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_gt,
	commutator = <, negator = <=,
	restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR >= (
	leftarg = usa_abartn, rightarg = usa_abartn, procedure = usa_abartn_ge,
	commutator = <=, negator = <,
	restrict = scalargtsel, join = scalargtjoinsel
);

-- aggregates
CREATE OR REPLACE FUNCTION usa_abartn_smaller(usa_abartn, usa_abartn)
RETURNS usa_abartn
AS '$libdir/usa_abartn'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION usa_abartn_larger(usa_abartn, usa_abartn)
RETURNS usa_abartn
AS '$libdir/usa_abartn'
    LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE min(usa_abartn)  (
    SFUNC = usa_abartn_smaller,
    STYPE = usa_abartn,
    SORTOP = <
);

CREATE AGGREGATE max(usa_abartn)  (
    SFUNC = usa_abartn_larger,
    STYPE = usa_abartn,
    SORTOP = >
);

-- btree operator class
CREATE OPERATOR CLASS usa_abartn_ops DEFAULT FOR TYPE usa_abartn USING btree AS
	OPERATOR 1 <,
	OPERATOR 2 <=,
	OPERATOR 3 =,
	OPERATOR 4 >=,
	OPERATOR 5 >,
	FUNCTION 1 usa_abartn_cmp(usa_abartn, usa_abartn);
-- cast from/to text
CREATE CAST (usa_abartn AS text) WITH FUNCTION usa_abartn_to_text(usa_abartn) AS ASSIGNMENT;
CREATE CAST (text AS usa_abartn) WITH FUNCTION text_to_usa_abartn(text) AS ASSIGNMENT;

/* Does this survive a pg_dump?
CREATE CAST (int        AS usa_abartn) WITHOUT FUNCTION AS ASSIGNMENT;
CREATE CAST (usa_abartn AS int)        WITHOUT FUNCTION;
*/
