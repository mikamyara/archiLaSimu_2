#include <regex>
#include <iostream>
#include <vector>


std::string 
trim(const std::string& str) {
    std::smatch matches;
    std::regex pattern1("^\\s*(.*?)\\s*$");
    if (std::regex_search(str, matches, pattern1))  {
        return matches.str(1);
    }
    return str;
}


std::string 
toUpper (std::string str)
{

    std::transform (str.begin (), str.end (), str.begin (),[](unsigned char c)
    {
        return std::toupper (c);
    }
                   );
    return str;
}

bool explode(const std::string input, std::vector < std::string > &result)
{
    std::regex rgx ("\\b\\w+\\b");

    std::sregex_iterator iter (input.begin (), input.end (), rgx);
    std::sregex_iterator end;

    while (iter != end)
    {
        result.push_back (iter->str ());
        ++iter;
    }
    return !result.empty ();
}
