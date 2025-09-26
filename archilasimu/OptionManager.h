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

#include <emscripten/fetch.h>
#include <string>
#include <atomic>
#include <string>
#include <map>
#include "imgui.h"
class ArchiLaSimuApp;

class OptionManager {
public:
    OptionManager(ArchiLaSimuApp* inApp);

    void fetchOptions();
    void renderLoadingPopup();
    void renderErrorPopup();

    std::map<std::string, std::string>  getOptionsContent();
    std::map<std::string, std::string>  parseOptions(std::string& text);

    std::atomic<bool> isDownloading;
    std::atomic<bool> isError;
    std::string optionsContent;
    std::map<std::string, std::string>  mOptions;
    bool loadingPopupOpened;
    bool errorPopupOpened;
    ArchiLaSimuApp* mApp;
    double startTime;  
};

// Global singleton for extern "C" callbacks
extern OptionManager* g_optionManagerInstance;

extern "C" {
    void optionsDownloadSucceeded(emscripten_fetch_t* fetch);
    void optionsDownloadFailed(emscripten_fetch_t* fetch);
}

