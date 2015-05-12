// Equations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Equation.h"
#include "StringExtensions.h"
int _tmain(int argc, _TCHAR* argv[])
{
	Equation eq = Equation();

	eq.SetEquation("'Vf'\"Final Velocity\" = 'Vi't\"Time\" + 0.5at^2");

	eq.SetVariable("Vi", 15.0);
	eq.SetVariable("t", 0.0016);
	eq.SetVariable("a", 4.0);

	try
	{
		std::cout << eq.GetFormattedEquation() << std::endl;
		std::cout << eq.GetAnswer("Vf") << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	getchar();

	return 0;
}

