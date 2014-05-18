#include <QuantPDE/Core>

#include <QuantPDE/Modules/Payoffs/Vanilla>      // Modules::call, Modules::put
#include <QuantPDE/Modules/Derivatives/Vanilla>  // Modules::EuropeanOption
#include <QuantPDE/Modules/Solvers/European>     // Modules::ImplicitEuropeanSolver

using namespace QuantPDE;
using namespace QuantPDE::Modules;

///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

	// Default options
	Real expiry       = 1.;
	Real interest     = 0.04;
	Real volatility   = 0.2;
	Real dividends    = 0.;
	Real stock        = 100.;
	Real strike       = 100.;
	unsigned refinement = 5;
	unsigned steps      = 25;
	bool isCall         = true;

	// Setting options with getopt
	{ char c;
	while((c = getopt(argc, argv, "d:hK:pr:R:s:S:T:v:")) != -1) {
		switch(c) {
			case 'd':
				dividends = atof(optarg);
				break;
			case 'h':
				cerr <<
"ConvergenceTableEuropean [OPTIONS]" << endl << endl <<
"Outputs the rate of convergence for computing the price of a European call or" << endl <<
"put using an upwind, fully implicit discretization of the Black-Scholes partial" << endl <<
"differential equation." << endl <<
endl <<
"-d REAL" << endl <<
endl <<
"    sets the dividend rate (default is 0.)" << endl <<
"-K REAL" << endl <<
endl <<
"    sets the strike price (default is 100.)" << endl <<
endl <<
"-p" << endl <<
endl <<
"    computes the price of a European put (default is call)" << endl <<
endl <<
"-r REAL" << endl <<
endl <<
"    sets interest rate (default is 0.04)" << endl <<
endl <<
"-R POSITIVE_INTEGER" << endl <<
endl <<
"    sets the maximum number of refinement steps in the computation (default is" << endl <<
"    5)" << endl <<
endl <<
"-s POSITIVE_INTEGER" << endl <<
endl <<
"    sets the initial number of steps to take in time (default is 25)" << endl <<
endl <<
"-S REAL" << endl <<
endl <<
"    sets the initial stock price (default is 100.)" << endl <<
endl <<
"-T POSITIVE_REAL" << endl <<
endl <<
"    sets the expiry time (default is 1.)" << endl <<
endl <<
"-v REAL" << endl <<
endl <<
"    sets the volatility" << endl << endl;
				return 0;
			case 'K':
				strike = atof(optarg);
				break;
			case 'p':
				isCall = false;
				break;
			case 'r':
				interest = atof(optarg);
				break;
			case 'R':
				refinement = atoi(optarg);
				break;
			case 's':
				steps = atoi(optarg);
				if(steps <= 0) {
					cerr <<
"error: the number of steps must be positive" << endl;
					return 1;
				}
				break;
			case 'S':
				stock = atof(optarg);
				break;
			case 'T':
				if((expiry = atof(optarg)) <= 0.) {
					cerr <<
"error: expiry time must be positive" << endl;
					return 1;
				}
				break;
			case 'v':
				volatility = atof(optarg);
				break;
		}
	} }

	// Setting up the table
	const int td = 10;
	cout
		<< setw(td) << "Nodes"  << "\t"
		<< setw(td) << "Steps"  << "\t"
		<< setw(td) << "Value"  << "\t"
		<< setw(td) << "Change" << "\t"
		<< setw(td) << "Ratio"
		<< endl
	;
	cout.precision(4);
	Real previousValue = nan(""), previousChange = nan("");

	// Initial discretization
	// TODO: Create grid based on initial stock price and strike price
	Axis S {
		0., 10., 20., 30., 40., 50., 60., 70.,
		75., 80.,
		84., 88., 92.,
		94., 96., 98., 100., 102., 104., 106., 108., 110.,
		114., 118.,
		123.,
		130., 140., 150.,
		175.,
		225.,
		300.,
		750.,
		2000.,
		10000.
	};

	// Payoff function
	std::function<Real (Real)> payoff = std::bind(
		isCall ? Payoffs::call : Payoffs::put,
		std::placeholders::_1,
		strike
	);

	// Alternatively, we could have used...
	//auto payoff = QUANT_PDE_MODULES_PAYOFFS_CALL_FIXED_STRIKE(strike);
	//auto payoff = QUANT_PDE_MODULES_PAYOFFS_PUT_FIXED_STRIKE(strike);

	for(unsigned l = 0; l < refinement; l++, steps *= 2) {

		///////////////////////////////////////////////////////////////
		// Build spatial grid
		///////////////////////////////////////////////////////////////

		S = S.refine();
		Grid1 G(S);

		///////////////////////////////////////////////////////////////
		// Build problem
		///////////////////////////////////////////////////////////////

		EuropeanOption europeanOption(
			payoff,
			[interest]   (Real, Real) { return interest;   },
			[volatility] (Real, Real) { return volatility; },
			[dividends]  (Real, Real) { return dividends;  },
			0., expiry
		);

		///////////////////////////////////////////////////////////////
		// Solve problem
		///////////////////////////////////////////////////////////////

		// ImplicitEuropeanSolver<> solver;

		///////////////////////////////////////////////////////////////
		// Table
		///////////////////////////////////////////////////////////////

		/*
		// Solution at S = 100.
		Real value = G.accessor(stepper.solution())(stock);

		// Change and ratio between successive solutions
		Real
			change = value - previousValue,
			ratio = previousChange / change
		;

		// Print out row of table
		cout
			<< scientific
			<< setw(td) << S.size()        << "\t"
			<< setw(td) << stepper.steps() << "\t"
			<< setw(td) << value           << "\t"
			<< setw(td) << change          << "\t"
			<< setw(td) << ratio
			<< endl
		;

		previousChange = change;
		previousValue = value;
		*/

	}

	return 0;
}

