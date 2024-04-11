#include <regex>
#include <iostream>
#include <vector>


std::string trim(const std::string& str) {
	std::smatch matches;
	std::regex pattern1("^\\s*(.*?)\\s*$");
	if (std::regex_search(str, matches, pattern1))  {
		return matches.str(1);
	} 
	return str; 
}
