#include "EquationRearranger.h"
#include "Equation.h"

EquationRearranger::EquationRearranger(const Equation& _equation)
		:equation(_equation)
{
	workingEquation = equation.brokenDownEquation;
}

EquationRearranger::~EquationRearranger()
{

}

std::vector<std::string> EquationRearranger::IsolateVariable(const std::string& variable)
{
	auto equalIter = std::find(workingEquation.begin(), workingEquation.end(), "=");
	auto varIter = std::find(workingEquation.begin(), workingEquation.end(), variable);

	if(equalIter == workingEquation.end() || varIter == workingEquation.end())
		throw std::exception("Equal Sign or Desired Variable Not Found");
	
	//need to add support for rearranging the equation here

	std::vector<std::string> parts;
	RearrangeEquation(varIter, parts);

	int iterDiff = equalIter - varIter;//distance between equal sign and variable

	if((iterDiff == 1 || iterDiff == -1) )//&& //Check that the variable is beside the equal sign and
		//(varIter - parts.begin() == 0 || parts.end() - varIter == 1)) //that it is at the very end or very beginning
	{
		//erase the variable and equal sign from the equation, so we only process what's needed
	/*	if(equalIter > varIter)
			parts.erase(parts.begin(), equalIter + 1);
		else
			parts.erase(equalIter, parts.end());*/
		parts.erase(parts.begin(), parts.begin()+2);
	}

	return parts;
}

void EquationRearranger::RearrangeEquation(const std::vector<std::string>::const_iterator& variablePos, std::vector<std::string>& outVal)
{



	outVal = workingEquation;
}

std::string EquationRearranger::InvertOperator(const std::string& operatorVal)
{
	if(operatorVal == "sqrt")
	{

	}

	return "";
}
