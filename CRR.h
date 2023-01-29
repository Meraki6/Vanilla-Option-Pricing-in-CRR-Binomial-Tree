#ifndef BINOMIAL_H
#define BINOMIAL_H

#ifndef CONST_ENUM
#define CONST_ENUM
#define	SUCCESS	0
#define	FAILURE	-1

enum	CALL_PUT	{Call, Put};
enum	AMER_EURO	{European, American};
#endif

#define OPTION_MULTIPLIER(x)	((x) == Call ? 1.0 : -1.0)
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

__declspec(dllexport) int CRRBinomialTree(
	CALL_PUT	callOrPut,				/* (I) put or call flag (use Call or Put) */
	AMER_EURO	amerOrEuro,				/* (I) option type (use European, American) */
	double		spotPrice,				/* (I) spot price of underlying */
	double		strike,					/* (I) strike price of option */
	double		maturity,				/* (I) maturity of option in years */
	double		vol,					/* (I) annual volatility of underlying */
	double		rate,					/* (I) annual continuous compounded discount rate */
	int			nStep,					/* (I) number of steps in tree */
	double		*value);				/* (O) option value */

#endif