// Equations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Equation.h"
#include "StringExtensions.h"
int _tmain(int argc, _TCHAR* argv[])
{
	Equation eq = Equation();
	//Set a sample equation. We can specify the name of the variables inside the declaration with double quotes
	//(backslashes for escape sequence)
	eq.SetEquation("'Vf'\"Final Velocity\" = 'Vi't\"Time\" + 0.5at^2");
	
	//Set the variables values
	eq.SetVariable("Vi", 15.0);
	eq.SetVariable("t", 0.0016);
	eq.SetVariable("a", 4.0);

	try
	{
		std::cout << "The Equation : 'Vf'\"Final Velocity\" = 'Vi't\"Time\" + 0.5at^2 after formatting:" << std::endl;
		//Display the equation after formatting
		std::cout << eq.GetFormattedEquation() << std::endl;
		
		std::cout << "\nCalculating Answer for Vf (Final Velocity) when\n" <<
			"Vi(Initial Velocity) = 15 meters per second\n" <<
			"t(Time) = 0.0016 seconds\n" <<
			"and a(Acceleration) = 4.0 meters per second squared\n" << std::endl;
		
		//Display the calculated result of Final Velocity	
		std::cout << "Answer: " << eq.GetAnswer("Vf") << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	getchar();

	return 0;
}

