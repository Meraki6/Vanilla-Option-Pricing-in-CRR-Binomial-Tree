#include "CRR.h"
#include <iostream>
#include <math.h>

using namespace std;

int CRRBinomialTree(
	CALL_PUT	callOrPut,				/* (I) put or call flag (use Call or Put) */
	AMER_EURO	amerOrEuro,				/* (I) option type (use European, American) */
	double		spotPrice,				/* (I) spot price of underlying */
	double		strike,					/* (I) strike price of option */
	double		maturity,				/* (I) maturity of option in years */
	double		vol,					/* (I) annual volatility of underlying */
	double		rate,					/* (I) annual continuous compounded discount rate */
	int			nStep,					/* (I) number of steps in tree */
	double		*value)					/* (O) option value */
/*-----------------------------------------------------------------------------
** FUNCTION:	CRRBinomialTree.
**
** DESCRIPTION:	Calculates the price of an option using the Cox, Ross and Rubinstein
**				binomial tree model.
**
** RETURNS:		SUCCESS and the premium, otherwise FAILURE.                                  
**
**---------------------------------------------------------------------------*/
{

	if (nStep <= 0)
	{
		cout << "CRRBinomialTree: Negative number of steps." << endl;
		return FAILURE;
	}            
	
	if (spotPrice <= 0.0 || strike <= 0.0 || maturity < 0.0 || vol < 0.0 || rate < 0.0)
	{
		cout << "CRRBinomialTree: Invalid input detected." << endl;
		return FAILURE;
	}

	// TO-BE-COMPLETED
	// define parameters needed first
	double u = exp(vol * sqrt(maturity / nStep)), d = 1 / u;
	double p = (exp(rate * maturity / nStep) - d) / (u - d), discount = exp(-rate * maturity / nStep);

	/*
	first construct stock and payoff at maturity with size nStep + 1
	stockOld and payoffOld are used as intermediary to renew the stock and payoff
	here we omit the construction process of stock price since stock prices at any paths can be calculated from the next path
	*/
	double* stock = new double[nStep + 1];
	double* payoff = new double[nStep + 1];
	double* stockOld = NULL, * payoffOld = NULL;

	// assign values to stock and payoff at nStep + 1
	for (size_t i = 0; i <= nStep; ++i) {
		stock[i] = i == 0 ? spotPrice * pow(u, nStep) : stock[i - 1] * pow(d, 2);
		payoff[i] = MAX(OPTION_MULTIPLIER(callOrPut) * (stock[i] - strike), 0);
	}

	/*
	trace back from nStep + 1 to 0 or from maturity to current time
	everytime
		use stockOld and payoffOld as intermediary to update values
		update sizes of stock and payoff to step
		update stock by multiplying d
		update payoff by discounting oldpayoff back and comparing with current exercise value if American
			*(payoffOld + i), same index with current, means upper price for current price, while *(payoffOld + i + 1), index plus 1, means lower price
			amerOrEuro is 1 if American, 0 otherwise, used to multiply the corresponding exercise value
	*/
	for (int step = nStep; step > 0; --step) {
		stockOld = stock;
		payoffOld = payoff;
		stock = new double[step];
		payoff = new double[step];
		for (size_t i = 0; i < step; ++i) {
			*(stock + i) = (*(stockOld + i)) * d;
			*(payoff + i) = MAX((*(payoffOld + i) * p + *(payoffOld + i + 1) * (1 - p)) * discount, amerOrEuro * MAX(OPTION_MULTIPLIER(callOrPut) * (*(stock + i) - strike), 0));
		}
		delete[] stockOld; // free dynamic arrays
		delete[] payoffOld;
	}

	// now both payoff and stock become array with size 1 and corresponding values are option premium and spot price
	*value = *payoff;

	return SUCCESS;
}