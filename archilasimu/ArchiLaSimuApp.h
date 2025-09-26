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

#include "imGuiApp.h"
#include "CircuitElements.h"
#include "ArchiCircuit.h"
#include "CPU.h"
#include "RAM.h"
#include "Assembleur.h"
#include "APropos.h"
#include "OptionManager.h"

class ArchiLaSimuApp : public imGuiApp {
public :
    ArchiLaSimuApp();
    virtual int customInit();
    virtual void mainLoop();
    void drawRect();
    GLuint CreateFramebuffer(int width,int height );
    void DrawTriangleInFramebuffer();
    void DrawFramebufferInImGui( );
    void drawMainWindow();
    void drawOtherWindow();
    void getOptions(std::map<std::string, std::string>& opt );

    ImFont *m_customFont;
    bool m_show_demo_window ;
    bool m_show_another_window;
    ImVec4 m_clear_color ;
    CPU* mCPU;
    RAM* mRAM;
    ExtBus* mExternalBus;
    Assembleur* mASM;
    OptionManager* mOptions;
    APropos* mAPropos;

    // software options
    int opt_disableLoadRam;
    int opt_disableSaveRam;
    int opt_disableLoadMicrocode;
    int opt_disableSaveMicrocode;

};

