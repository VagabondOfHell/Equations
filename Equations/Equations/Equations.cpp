// Equations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Equation.h"
#include "StringExtensions.h"
int _tmain(int argc, _TCHAR* argv[])
{
	Equation eq = Equation();

	eq.SetEquation("y = -(z + -b)");
	eq.SetEquation("t= -g +(-yb + -m)");

	eq.SetVariable("y", 5);
	eq.SetVariable("g", 9);

	eq.SetVariable("m", 3);
	eq.SetVariable("x", 7);
	eq.SetVariable("b", 4);

	std::cout << eq.GetAnswer("t") << std::endl;

	getchar();

	return 0;
}

