#include "EquationParser.h"
#include "StringExtensions.h"

using namespace EquationSharedData;

bool EquationParser::ValidateEquation(const std::string& equation)
{
	int numBrackets = 0;
	int numSingleQuotes = 0;
	int numDoubleQuotes = 0;
	int numEqualSigns = 0;

	bool setHasBrackets = false;

	for (unsigned int i = 0; i < equation.size(); i++)
	{
		switch (equation[i])
		{
		case '(':
			numBrackets++;
			setHasBrackets = true;
			break;
		case ')':
			numBrackets--;
			break;
		case '\'':
			numSingleQuotes++;
			break;
		case '"':
			numDoubleQuotes++;
			break;
		case '=':
			numEqualSigns++;
			break;
		default:
			break;
		}
	}

	//Check if brackets are opened and closed, there is only one equal sign, and each type of quote is opened and closed
	return (numBrackets == 0 && numEqualSigns == 1 && numSingleQuotes % 2 == 0 && numDoubleQuotes % 2 == 0);
}

std::vector<std::string> EquationParser::ExtractEquationParts(const std::string& equation)
{
	//NOTE: In V2, look at not erasing in order to improve performance (index position and look forward for example)
	
	//This will be our working copy that we can modify (when there are no more characters, we will be done extraction)
	std::string copyVal = equation;
	
	//Each required section will be stored here
	std::vector<std::string> retVal;

	while (!copyVal.empty())
	{
		//Remove Spaces we come across
		if(StringExtensions::IsSpace(copyVal[0]))
			copyVal.erase(0, 1);
		else if(StringExtensions::IsLetter(copyVal[0]))
		{
			std::string wordOperator;
			//If the last item added was a variable, insert a multiplication sign before adding this letter
			if(retVal.size() > 0 && InsertMultiplicationSymbol(retVal[retVal.size() - 1]))
				retVal.push_back("*");
			//Check if this letter is the beginning of a word operator (i.e. sin, cos, etc.)
			if(!IsWordOperator(copyVal, wordOperator))
			{
				//if not, add the letter and remove from the processing string
				std::string temp; temp += copyVal[0];
				retVal.push_back(temp);
				copyVal.erase(0, 1);
			}
			else
				//otherwise add the word operator (IsWordOperator removes from our working string)
				retVal.push_back(wordOperator);
		}
		else if(StringExtensions::IsPunctuation(copyVal[0]))
		{
			std::string outVal;
			//If there are surrounding single quotes it is a multi-char variable (i.e. Vf for final velocity)
			if(copyVal[0] == '\'' && GetBetweenSymbol(copyVal, '\'', outVal))
			{
				retVal.push_back(outVal);
			}
			//If there are double quotes it is a name for a variable and is removed from equation processing
			else if(copyVal[0] == '"' && GetBetweenSymbol(copyVal, '"', outVal))
			{
				//don't do anything, because the GetBetweenSymbol removes it for us
			}
			else//Any other kind of punctuation
			{
				std::string temp; temp += copyVal[0];
				
				//If we just opened brackets, check if a multiplication symbol should be added first
				if(temp == "(" && retVal.size() > 0 && InsertMultiplicationSymbol(retVal[retVal.size() - 1]))
					retVal.push_back("*");
					
				//In V2, have list of valid operators (i.e. ! and ? are currently invalid, and therefore should not
				//be accepted)
				//Otherwise just add the punctuation
				retVal.push_back(temp);
				copyVal.erase(0, 1);
			}
		}
		else if(StringExtensions::IsNumber(copyVal[0]))
		{
			std::string numVal;
			//Convert the number to a string and add to the vector
			if(GetNumberString(copyVal, numVal))
				retVal.push_back(numVal);
		}
	}

	return retVal;
}

bool EquationParser::InsertMultiplicationSymbol(const std::string& prevInsert)
{
	//Check to see if we should insert a multiplication symbol before this variable
	//i.e. y = mx + b will be treated y = m*x+b.    5(x-b) will be 5 * (x-b)
	if((!StringExtensions::IsAllAlphanumeric(prevInsert) && !StringExtensions::IsAllNumbers(prevInsert, true))
		&&	prevInsert != ")")
		return false;

	if(IsWordOperator(prevInsert))
		return false;

	return true;
}

bool EquationParser::IsArithmeticOperator(const std::string& phrase)
{
	if(phrase.size() == 1)
	{
		if(phrase[0] == '*' || phrase[0] == '/' || phrase[0] == '+' ||
			phrase[0] == '-' || phrase[0] == '%' || phrase[0] == '=')
			return true;
	}

	if(phrase.size() > 1 && IsWordOperator(phrase))
		return true;

	return false;
}

std::vector<std::string>::const_iterator 
	EquationParser::FindFirstWordOperator(const std::vector<std::string>& vecToSearch)
{
	//Searchs the vector of strings for the first word operator (i.e. Sin, Cos, etc.)
	return std::find_first_of(vecToSearch.begin(), vecToSearch.end(),
		WORD_OPERATORS.begin(), WORD_OPERATORS.end());
}

bool EquationParser::ExtractVariables(std::vector<std::string>& brokenDownEquation, VariableMap& mapToFill)
{
	Variable* lastVariableCreated = NULL;

	unsigned int constantsIndex = 0;

	for (unsigned int i = 0; i < brokenDownEquation.size(); i++)
	{
		//If the first character of the currently evaluated string is double quotes
		if(brokenDownEquation[i][0] == '"')
		{
			//We have a name, and will add that name to the last variable created (name comes after variable)
			if(lastVariableCreated)
				lastVariableCreated->Name = StringExtensions::RemoveAllOfSymbol(brokenDownEquation[i], '"');
		}
		//This step fails if a decimal is part of the string
		else if(StringExtensions::IsAllAlphanumeric(brokenDownEquation[i]) && !IsWordOperator(brokenDownEquation[i]))
		{
			//If the currently evaluated string is a letter/word and not a word operator(sin, cos, etc)
			//Then attempt to insert into map and store as the last variable created
			if(!StringExtensions::IsAllNumbers(brokenDownEquation[i], true))
				lastVariableCreated = &mapToFill.insert
					(VariableMap::value_type(brokenDownEquation[i], Variable(brokenDownEquation[i]))).first->second;
			else
			{
				//If the string is all numbers, add a variable that we will declare as a Constant for later reference
				std::string constName = "con" + std::to_string(constantsIndex);
				lastVariableCreated = &mapToFill.insert
					(VariableMap::value_type(constName, Variable(std::stod(brokenDownEquation[i]), constName))).first->second;
				brokenDownEquation[i] = constName;
				constantsIndex++;
			}
		}
		//And because the above fails if it contains a decimal, we place this here
		else if(StringExtensions::IsAllNumbers(brokenDownEquation[i], true))
		{
			//Same as above...if the string is all numbers, add a variable that we will declare as a Constant for later reference
			//TODO: in V2, move this into its own function...don't know why it wasn't already
			std::string constName = "con" + std::to_string(constantsIndex);
			lastVariableCreated = &mapToFill.insert
				(VariableMap::value_type(constName, Variable(std::stod(brokenDownEquation[i]), constName))).first->second;
			brokenDownEquation[i] = constName;
			constantsIndex++;
		}
	}

	return true;
}

bool EquationParser::EvaluatePhrase(std::string& equation, const std::string& phrase)
{
	//Evaluates a small substring. This is mainly used when checking for word operators in the equation pre-processing
	//For example, if Sqrt is the phrase, it will grab the next four characters of the passed equation starting point
	//and check if that substring equals sqrt
	
	std::string testString = equation.substr(0, phrase.size());

	if(testString.find(phrase.c_str(), 0, phrase.size()) != std::string::npos)
	{
		equation.erase(0, phrase.size());
		return true;
	}

	return false;
}

bool EquationParser::IsWordOperator(std::string& equation, std::string& foundOperator)
{
	//For each Word Operator we have, check if the passed section of the equation contains it
	for (unsigned int i = 0; i < WORD_OPERATORS.size(); i++)
	{
		if(EvaluatePhrase(equation, WORD_OPERATORS[i]))
		{
			foundOperator = WORD_OPERATORS[i];
			return true;
		}
	}

	return false;
}

bool EquationParser::IsWordOperator(const std::string& phrase)
{
	//Check for a word operator in the passed string
	for (unsigned int i = 0; i < WORD_OPERATORS.size(); i++)
	{
		//Search from the beginning of the phrase to the length of the word operator
		if(StringExtensions::FindToPoint(phrase, WORD_OPERATORS[i], WORD_OPERATORS[i].size()))
			return true;
	}

	return false;
}

//look at returning a pair in V2
bool EquationParser::GetBetweenSymbol(std::string& baseString, const char symbol, std::string& outVal)
{
	std::string::size_type firstSymbol = baseString.find_first_of(symbol);

	if(firstSymbol == std::string::npos)
		return false;

	std::string::size_type secondSymbol = baseString.find_first_of(symbol, firstSymbol + 1);

	if(secondSymbol == std::string::npos)
		return false;
	
	//Take only what is within the symbols (exclude the surrounding quotes for example)
	std::string section = baseString.substr(firstSymbol + 1, secondSymbol - 1);
	baseString.erase(firstSymbol, secondSymbol+1);
	
	//Set the return variable
	outVal = section;
	return true;
}

bool EquationParser::GetNumberString(std::string& baseString, std::string& outVal)
{
	bool decimalFound = false;

	unsigned int i = 0;
	//find how far the number expands to (how many digits + decimal)
	for (i; i < baseString.size(); i++)
	{
		if(!StringExtensions::IsNumber(baseString[i]) && !(baseString[i] == '.' && !decimalFound))
			break;
	}
	
	//Pull the substring of the number out
	if(i != 0)
	{
		outVal = baseString.substr(0, i);
		baseString.erase(0, i);

		return true;
	}

	return false;
}
