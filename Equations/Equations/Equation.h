#pragma once
#include "Variable.h"

#include <map>
#include <vector>
#include <array>

class Equation
{
private:
	typedef std::map<std::string, Variable> VariableMap;
	typedef std::vector<std::string> StringVector;

	bool hasBrackets;
	
	///<summary>Checks to see if the passed equation is formed correctly</summary>
	///<param name="equation">The string representing the equation</param>
	///<returns>True if valid, false if not</returns>
	bool ValidateEquation(const std::string& equation)const;

	///<summary>Processes a vector that represents an equation</summary>
	///<param name="portion">The portion of the equation to solve for. Will be modified by this method
	///(Perhaps modify so that the parameter isn't modified)</param>
	///<param name="tlIndex">The temporary value index, used as placeholders for solved portions</param>
	///<returns>The remainder of the modifications</returns>
	StringVector ProcessEquation(StringVector& portion, unsigned int& tlIndex);

	void ProcessBrackets(StringVector& equation, unsigned int& tlIndex);

	void ProcessOperator(StringVector& portion, unsigned int& tlIndex, StringVector::iterator& operatorIter);

protected:
	struct OperatorVariables
	{
		std::vector<std::string>::const_iterator lhValIter;
		std::vector<std::string>::const_iterator rhValIter;
		double lhVal;
		double rhVal;

		OperatorVariables()
			:lhVal(0), rhVal(0)
		{

		}

		OperatorVariables(std::vector<std::string>::const_iterator lhIter, 
			std::vector<std::string>::const_iterator rhIter, double leftVal, double rightVal)
			: lhValIter(lhIter), rhValIter(rhIter), lhVal(leftVal), rhVal(rightVal)
		{

		}
	};

	static std::array<std::string, 3> PRIORITY_3_OPERATORS;//multiplication, modulo, and division
	static std::array<std::string, 2> PRIORITY_4_OPERATORS;//addition and subtraction

	///<summary>Gets the position needed to erase the variable from working parts. 
	///If working on left hand value, returns beginning of variable, if working on 
	///right hand value, returns the end of variable, in both cases the opposite position
	///is given by the opPos</summary>
	///<param name="opPos">Position of the operator we are collecting the variables around</param>
	///<param name="lookLeft">True to look to the left of the operator, false to look to the right</param>
	///<returns>Iterator representing the beginning of the variable if lookLeft is true, 
	///otherwise returns iterator representing ending of the variable</returns>
	std::vector<std::string>::const_iterator GetVariablePosition(const StringVector& vec,
		const StringVector::const_iterator& opPos, bool lookLeft)const;  

	double ExecuteOperator(const double lhVal, const double rhVal, const std::string& operatorStr)const;

	OperatorVariables GetOperatorValues(const StringVector& portion, 
		const StringVector::const_iterator& operatorPos)const;

	StringVector IsolateVariable(const std::string& variable)const;

	StringVector brokenDownEquation;
	VariableMap variables;
	VariableMap workingVariables;

	std::string baseEquation;

public:
	Equation(void);
	virtual ~Equation(void);

	bool SetEquation(const std::string& equation);

	bool SetVariable(const std::string& varAlias, double varVal);

	bool GetVariable(const std::string& varAlias, Variable& outVal)const;

	double GetAnswer(const std::string& varToSolve);
};

