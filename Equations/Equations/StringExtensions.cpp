#include "StringExtensions.h"

#include <sstream>

std::vector<std::string> StringExtensions::ParseAllWhiteSpace(const std::string& stringToParse)
{
	std::vector<std::string> stringParts;
	std::stringstream currString(stringToParse);
	std::string temp;

	while (getline(currString, temp, ' '))
	{
		if(!temp.empty())
			stringParts.push_back(temp);
	}

	return stringParts;
}
