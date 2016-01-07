#include "postgres.h"
#include "fmgr.h"
#include "libpq/pqformat.h"
#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef uint32 usa_abartn_t;

#define USAABARTNGetDatum(x)	 Int32GetDatum(x)
#define DatumGetUSAABARTN(x)	 DatumGetInt32(x)
#define PG_GETARG_USAABARTN(n) DatumGetUSAABARTN(PG_GETARG_DATUM(n))
#define PG_RETURN_USAABARTN(x) return USAABARTNGetDatum(x)

//#define DIGITTOINT(n)	(((n) >= '0' && (n) <= '9') ? (int32) ((n) - '0') : 0)

Datum usa_abartn_in(PG_FUNCTION_ARGS);
Datum usa_abartn_out(PG_FUNCTION_ARGS);
Datum usa_abartn_to_text(PG_FUNCTION_ARGS);
Datum text_to_usa_abartn(PG_FUNCTION_ARGS);
Datum usa_abartn_send(PG_FUNCTION_ARGS);
Datum usa_abartn_recv(PG_FUNCTION_ARGS);
Datum usa_abartn_lt(PG_FUNCTION_ARGS);
Datum usa_abartn_le(PG_FUNCTION_ARGS);
Datum usa_abartn_eq(PG_FUNCTION_ARGS);
Datum usa_abartn_ne(PG_FUNCTION_ARGS);
Datum usa_abartn_ge(PG_FUNCTION_ARGS);
Datum usa_abartn_gt(PG_FUNCTION_ARGS);
Datum usa_abartn_cmp(PG_FUNCTION_ARGS);

static usa_abartn_t cstring_to_usa_abartn(char *usa_abartn_string);
static char *usa_abartn_to_cstring(usa_abartn_t usa_abartn);



/* generic input/output functions */
PG_FUNCTION_INFO_V1(usa_abartn_in);
Datum
usa_abartn_in(PG_FUNCTION_ARGS)
{
	usa_abartn_t	result;

	char   *usa_abartn_str = PG_GETARG_CSTRING(0);
	result = cstring_to_usa_abartn(usa_abartn_str);

	PG_RETURN_USAABARTN(result);
}

PG_FUNCTION_INFO_V1(usa_abartn_out);
Datum
usa_abartn_out(PG_FUNCTION_ARGS)
{
	usa_abartn_t	packed_usa_abartn;
	char   *usa_abartn_string;

	packed_usa_abartn = PG_GETARG_USAABARTN(0);
	usa_abartn_string = usa_abartn_to_cstring(packed_usa_abartn);

	PG_RETURN_CSTRING(usa_abartn_string);
}

/* type to/from text conversion routines */
PG_FUNCTION_INFO_V1(usa_abartn_to_text);
Datum
usa_abartn_to_text(PG_FUNCTION_ARGS)
{
	char	*usa_abartn_string;
	text	*usa_abartn_text;

	usa_abartn_t	packed_usa_abartn =  PG_GETARG_USAABARTN(0);

	usa_abartn_string = usa_abartn_to_cstring(packed_usa_abartn);
	usa_abartn_text = DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(usa_abartn_string)));

	PG_RETURN_TEXT_P(usa_abartn_text);
}

PG_FUNCTION_INFO_V1(text_to_usa_abartn);
Datum
text_to_usa_abartn(PG_FUNCTION_ARGS)
{
	text  *usa_abartn_text = PG_GETARG_TEXT_P(0);
	char  *usa_abartn_str = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(usa_abartn_text)));
	usa_abartn_t usa_abartn = cstring_to_usa_abartn(usa_abartn_str);

	PG_RETURN_USAABARTN(usa_abartn);
}

/* Functions to convert to/from bytea */
PG_FUNCTION_INFO_V1(usa_abartn_send);
Datum
usa_abartn_send(PG_FUNCTION_ARGS)
{
	StringInfoData buffer;
	usa_abartn_t usa_abartn = PG_GETARG_USAABARTN(0);

	pq_begintypsend(&buffer);
	pq_sendint(&buffer, (int32)usa_abartn, 4);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buffer));
}

PG_FUNCTION_INFO_V1(usa_abartn_recv);
Datum usa_abartn_recv(PG_FUNCTION_ARGS)
{
	usa_abartn_t	usa_abartn;
	StringInfo	buffer = (StringInfo) PG_GETARG_POINTER(0);

	usa_abartn = pq_getmsgint(buffer, 4);
	PG_RETURN_USAABARTN(usa_abartn);
}

/* functions to support btree opclass */
PG_FUNCTION_INFO_V1(usa_abartn_lt);
Datum
usa_abartn_lt(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL(a < b);
}

PG_FUNCTION_INFO_V1(usa_abartn_le);
Datum
usa_abartn_le(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL (a <= b);
}

PG_FUNCTION_INFO_V1(usa_abartn_eq);
Datum
usa_abartn_eq(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL(a == b);
}

PG_FUNCTION_INFO_V1(usa_abartn_ne);
Datum
usa_abartn_ne(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL(a != b);
}

PG_FUNCTION_INFO_V1(usa_abartn_ge);
Datum
usa_abartn_ge(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL(a >= b);
}

PG_FUNCTION_INFO_V1(usa_abartn_gt);
Datum
usa_abartn_gt(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);
	PG_RETURN_BOOL(a > b);
}

PG_FUNCTION_INFO_V1(usa_abartn_cmp);
Datum
usa_abartn_cmp(PG_FUNCTION_ARGS)
{
	usa_abartn_t a = PG_GETARG_USAABARTN(0);
	usa_abartn_t b = PG_GETARG_USAABARTN(1);

	PG_RETURN_INT32(a - b);
}

static int32
usa_abartn_cmp_internal(usa_abartn_t a, usa_abartn_t b)
{
    return a - b;

    /*if (a < b)
        return -1;
    else if (a > b)
        return 1;

    return 0;*/
}

/*****************************************************************************
 * Aggregate functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(usa_abartn_smaller);

Datum
usa_abartn_smaller(PG_FUNCTION_ARGS)
{
   usa_abartn_t left  = PG_GETARG_USAABARTN(0);
   usa_abartn_t right = PG_GETARG_USAABARTN(1);
   usa_abartn_t result;

   result = usa_abartn_cmp_internal(left, right) < 0 ? left : right;
   PG_RETURN_TEXT_P(result);
}

PG_FUNCTION_INFO_V1(usa_abartn_larger);

Datum
usa_abartn_larger(PG_FUNCTION_ARGS)
{
   usa_abartn_t left  = PG_GETARG_USAABARTN(0);
   usa_abartn_t right = PG_GETARG_USAABARTN(1);
   usa_abartn_t result;

   result = usa_abartn_cmp_internal(left, right) > 0 ? left : right;
   PG_RETURN_TEXT_P(result);
}


/*
 * Convert a cstring to an USAABARTN, validating the input.
 * Input in form AAAABBBBC is accepted.
 */
static usa_abartn_t
cstring_to_usa_abartn(char *usa_abartn_str)
{
    char            *c;
    usa_abartn_t    result;
    int             ndigits = 0;
    int             digit_value;
    int             weight1;
    int             weight2;
    int             weight3;
    int             check_digit_calc;
    int32           federal_reserve_routing_symbol,
                    aba_institution_identifier,
                    check_digit;

	federal_reserve_routing_symbol = aba_institution_identifier = check_digit = result = 0;

	for (c = usa_abartn_str; *c != 0; c++)
	{
		if (*c >= '0' && *c <= '9')
		{
            digit_value = *c - '0';

            result = result * 10 + digit_value;

			if (ndigits < 4) federal_reserve_routing_symbol = federal_reserve_routing_symbol * 10 + digit_value;
			else if (ndigits < 8) aba_institution_identifier = aba_institution_identifier * 10 + digit_value;
			else if (ndigits < 9) check_digit = check_digit * 10 + digit_value;

            switch (ndigits) {
                case 0:
                    weight1 = digit_value;
                break;
                case 1:
                    weight2 = digit_value;
                    switch (federal_reserve_routing_symbol) {
                        case 0: // United States Government
                        case 1: // Boston
                        case 2: // New York
                        case 3: // Philadelphia
                        case 4: // Cleveland
                        case 5: // Richmond
                        case 6: // Atlanta
                        case 7: // Chicago
                        case 8: // St. Louis
                        case 9: // Minneapolis
                        case 10: // Kansas City
                        case 11: // Dallas
                        case 12: // San Francisco
                        case 21: // Thrift Boston
                        case 22: // Thrift New York
                        case 23: // Thrift Philadelphia
                        case 24: // Thrift Cleveland
                        case 25: // Thrift Richmond
                        case 26: // Thrift Atlanta
                        case 27: // Thrift Chicago
                        case 28: // Thrift St. Louis
                        case 29: // Thrift Minneapolis
                        case 30: // Thrift Kansas City
                        case 31: // Thrift Dallas
                        case 32: // Thrift San Francisco
                        case 61: // Electronic Boston
                        case 62: // Electronic New York
                        case 63: // Electronic Philadelphia
                        case 64: // Electronic Cleveland
                        case 65: // Electronic Richmond
                        case 66: // Electronic Atlanta
                        case 67: // Electronic Chicago
                        case 68: // Electronic St. Louis
                        case 69: // Electronic Minneapolis
                        case 70: // Electronic Kansas City
                        case 71: // Electronic Dallas
                        case 72: // Electronic San Francisco
                        case 80: // traveler's cheques
                        break;
                        default:
                            ereport(ERROR,
                            (errmsg("The RTN part of the ABA routing transit number %s is invalid", usa_abartn_str)));
                        break;
                    }
                break;
                case 2:
                    weight3 = digit_value;
                break;
                case 3:
                case 6:
                    weight1 = weight1 + digit_value;
                break;
                case 4:
                case 7:
                    weight2 = weight2 + digit_value;
                break;
                case 5:
                    weight3 = weight3 + digit_value;
                break;
                case 8:
                    check_digit_calc = (weight1 * 7 + weight2 * 3 + weight3 * 9) % 10;
                    if (check_digit != check_digit_calc)
                        ereport(ERROR,
                            (errmsg("ABA routing transit number %s is invalid %d", usa_abartn_str, check_digit_calc)));
                break;
            }

            ndigits++;
		}
		else
			ereport(ERROR,
					(errmsg("Unexpected character '%c' in input data %s", *c, usa_abartn_str),
					 errhint("A valid ABA routing transit number consists of digits only.")));

	}
	if (ndigits != 9)
		ereport(ERROR,
				(errmsg("Invalid number of digits (%d) in input data %s", ndigits, usa_abartn_str),
				 errhint("A valid ABA routing transit number consists of 9 digits.")));

	PG_RETURN_USAABARTN(result);

}

/* Convert the internal representation to the AAAABBBBC output string */
static char *
usa_abartn_to_cstring(usa_abartn_t usa_abartn)
{
    int32   ndigits;
	char   *usa_abartn_str = palloc(10);

	if (usa_abartn < 0)
		/* Error out */;

    if ((ndigits = snprintf(usa_abartn_str, 10, "%09d", usa_abartn)) != 9)
		ereport(ERROR,
				(errmsg("invalid size (%d) of in input data %s", ndigits, usa_abartn_str),
				 errhint("Valid ABA routing transit number consists of 9 digits")));

	return usa_abartn_str;
}
