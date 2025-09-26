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
#pragma once

#include <string>


std::string trim(const std::string& str);
std::string toUpper (std::string inStr);
bool explode(const std::string input, std::vector < std::string > &result);
std::vector<std::string> splitString(const std::string& input, char separator);
extern "C" void get_full_url_into(char* buffer, int maxLen);
extern "C" void get_base_url_into(char* buffer, int maxLen);