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
#include "MM_imgui.h"
#include <string>


class imGuiApp {
public:
    imGuiApp();
    virtual int Initialize();
    virtual int customInit ();
    virtual int Run();
    virtual void mainLoop();


    ImGuiIO* m_io;
    std::string m_glsl_version;
    GLFWwindow* m_window;

    std::string mWindowTitle;

    static int sFrameRate;
    static int shidpiScale;

};


ImVec2  toHD(ImVec2 inPt);
int toHD(int inV) ;
ImVec2  toLD(ImVec2 inPt);
int toLD(int inV);
