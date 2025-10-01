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

#include "OptionManager.h"
#include <cstring>  
#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <sstream>
#include <emscripten/emscripten.h> 
#include "ArchiLaSimuApp.h"
#include "stringUtils.h"

OptionManager* g_optionManagerInstance = nullptr; // global pointer on the class for callbacks

// Callbacks extern "C"
extern "C" {

void optionsDownloadSucceeded(emscripten_fetch_t* fetch) {
    auto g = g_optionManagerInstance;
    if (!g) return;
    try {
        g->optionsContent.assign(fetch->data, fetch->numBytes);
        emscripten_fetch_close(fetch);
        g->mOptions = g->parseOptions(g->optionsContent);
        g->mApp->getOptions(g->mOptions);
    }
    catch(...) {
        g->isDownloading = false;
        g->isError = true;
        return;
    }
    //std::cout << g->optionsContent << "\n";
    g->isDownloading = false;
    g->isError = false;
    //std::cout << "[Callback] Download succeeded\n";
}

void optionsDownloadFailed(emscripten_fetch_t* fetch) {
    if (!g_optionManagerInstance) return;
    emscripten_fetch_close(fetch);
    g_optionManagerInstance->isDownloading = false;
    g_optionManagerInstance->isError = true;
    //std::cout << "[Callback] Download failed\n";
}

} // extern "C"

OptionManager::OptionManager(ArchiLaSimuApp* inApp)
{
    isDownloading = false;
    isError = false;
    errorPopupOpened = false;
    optionsContent = "";
    startTime = 0.0;
    loadingPopupOpened = false;
    g_optionManagerInstance = this;
    mApp = inApp;

}

void OptionManager::fetchOptions() {


    isDownloading = true;
    isError = false;
    startTime = emscripten_get_now();

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = optionsDownloadSucceeded;
    attr.onerror = optionsDownloadFailed;
    char URL[256]; get_base_url_into(URL,256);
    std::string str = URL;
    str += "/admin/options.txt";

    // cache busting
    double now = emscripten_get_now(); // millisecondes
    str += "?nocache=" + std::to_string((int)now); // ou utilise rand() si tu préfères


    emscripten_fetch(&attr,str.c_str());
}

void OptionManager::renderLoadingPopup() {

    if (isError) return;

    double now = emscripten_get_now();
    bool elapsed = (now - startTime) >= 1500.0; // Global singleton for extern "C" callbacks

    if (!elapsed && !isDownloading) 
        {
        ImGui::OpenPopup("Options");

        if (ImGui::BeginPopupModal("Options", NULL,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse ))
        {
            ImGui::Text("Téléchargement des options");

            ImGui::EndPopup();
        }

    }
}

void OptionManager::renderErrorPopup() {
    if (isError) 
    {
        ImGui::OpenPopup("Erreur");

        if (ImGui::BeginPopupModal("Erreur", NULL,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse ))
        {
            ImGui::Text("Erreur en chargeant les paramètres.\nMerci d'essayer à nouveau ultérieurement.");

            ImGui::EndPopup();
        }

    }
}

std::map<std::string, std::string>  OptionManager::getOptionsContent() {
    return mOptions;
}


std::map<std::string, std::string>  OptionManager::parseOptions(std::string& content)
{

    std::map<std::string, std::string> result;
    std::regex pattern(R"(^\s*([^\s=]+)\s*=\s*(\S+)\s*$)");
    std::smatch match;
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line))
    {
        if (std::regex_match(line, match, pattern))
        {
            std::string key = match[1];
            std::string value = match[2];
            result[key] = value;
        }
    }

    return result;
}


