/*
 * ArchiLaSimu - A Web navigator based Von-Neuman Architecture simulator 
 * Copyright (C) 2025 Mikhaël Myara, Arnaud Virazel - Université de Montpellier
 *
 * Ce programme est un logiciel libre : vous pouvez le redistribuer et/ou
 * le modifier selon les termes de la Licence Publique Générale GNU publiée
 * par la Free Software Foundation, soit la version 3 de la licence, soit
 * (à votre choix) toute version ultérieure.
 *
 * Ce programme est distribué dans l’espoir qu’il sera utile,
 * mais SANS AUCUNE GARANTIE, même implicite.
 * Voir la Licence Publique Générale GNU pour plus de détails.
 *
 * Vous devriez avoir reçu une copie de la Licence Publique Générale GNU
 * avec ce programme. Si ce n’est pas le cas, consultez <https://www.gnu.org/licenses/>.
 */

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



std::vector<std::string> splitString(const std::string& input, char separator) {
    // Crée une expression régulière pour le séparateur fourni
    std::string separatorPattern = std::string(1, separator);
    std::regex regexPattern(separatorPattern);

    // Utilise regex_token_iterator pour séparer la chaîne
    std::vector<std::string> result;
    std::sregex_token_iterator iter(input.begin(), input.end(), regexPattern, -1);
    std::sregex_token_iterator end;

    for (; iter != end; ++iter) {
        result.push_back(*iter);
    }

    return result;
}
