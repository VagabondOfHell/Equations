#pragma once
#include "EquationSharedData.h"

class EquationSolver
{
private:
	unsigned int tempValIndex;

	EquationSharedData::VariableMap workingVariables;
	EquationSharedData::StringVector workingEquation;

	inline bool GetVariable(const std::string& varAlias, Variable& outVal)const
	{
		EquationSharedData::VariableMap::const_iterator result = workingVariables.find(varAlias);

		if((result = workingVariables.find(varAlias)) != workingVariables.end())
		{
			outVal = result->second;
			return true;
		}

		return false;
	}

	///<summary>Gets the iterator position of the desired variable in the working equation vector</summary>
	///<param name="vec">The vector representing the equation portion to retrieve values from</param>
	///<param name="opPos">Position of the currently evaluated operator</param>
	///<param name="lookLeft">True to get the variable on the left of the operator, false to get
	///the variable on the right of the operator</param>
	///<returns>A vector iterator representing the beginning of the variable. If going to the left of the 
	///operator, this could be a variable or a negative sign. If going to the right it will always be a variable
	///but may be one after a negative sign</returns>
	std::vector<std::string>::const_iterator GetVariablePosition(const std::vector<std::string>& vec,
		const std::vector<std::string>::const_iterator& opPos, bool lookLeft)const; 

	///<summary>Gets the variables on the left and right side of the operator</summary>
	///<param name="portion">The portion of the equation to process</param>
	///<param name="operatorPos">The position of the operator being evaluated</param>
	///<returns>An OperatorVariables struct that holds the beginning iterator of the left variable
	///and the end iterator of the right variable, as well as booleans representing iterator validity</returns>
	EquationSharedData::OperatorVariables GetOperatorValues(const EquationSharedData::StringVector& portion, 
		const EquationSharedData::StringVector::const_iterator& operatorPos)const;
	
	///<summary>Executes the operator specified by operatorStr on the two values provided</summary>
	///<param name="lhVal">The value on the left side of the operator</param>
	///<param name="rhVal">The value on the right side of the operator</param>
	///<param name="operatorStr">The string representing which operator to execute</param>
	///<returns>A double representing the answer</returns>
	double ExecuteOperator(const double lhVal, const double rhVal, const std::string& operatorStr)const;
	
	///<summary>Processes the portion of equation within brackets. Done this way to avoid recursion</summary>
	///<param name="equation">The equation to parse for brackets. If no brackets, this method does nothing </param>
	void ProcessBrackets(std::vector<std::string>& equation);

	///<summary>Multi-pass method that parses the vector-represented equation based on the 
	///Order of Operations.</summary>
	///<param name="portion">The vector representing the equation</param>
	///<returns>A collection of strings representing the answer of the portion.
	///Generally one in length, unless it requires a negative operator, in which case its size is 2</returns>
	std::vector<std::string> ProcessEquation(std::vector<std::string>& portion);

	///<summary>Processes the section of the equation surrounding the specified operator</summary>
	///<param name="portion">The representation of the equation in the form of a vector</param>
	///<param name="operatorIter">The iterator representing the position of the operator in the vector parameter</param>
	void ProcessOperator(std::vector<std::string>& portion, const std::vector<std::string>::const_iterator& operatorIter);

public:
	
	EquationSolver();
	EquationSolver(const std::vector<std::string>& brokenDownEquation, 
		const std::map<std::string, Variable>& extractedVariables);

	~EquationSolver();

	double Solve();
};

