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


#include <imgui.h>
#include "ArchiCircuit.h"
#include "Sequencer.h"
#include "Assembleur.h"
#include "CodeUploadDownload.h"

class RAM;

class CPU {
public :
    CPU();
    void draw(ImDrawList* dl, ImVec2 window_pos);
    void drawWidgets(ImDrawList* dl,ImVec2 pos);
    void getAdressingModes(std::vector<std::string>& shortName,std::vector<std::string>& longName);
    void Rebuild();
    void Reset();
    void setRegisterValue(std::string regName,int inValue);
    int  getRegisterValue(std::string regName);
    void executeArchiOrders( const std::vector<std::string>& inOrders,bool hilite);
    void hiliteArchiOrders( const std::vector<std::string>& inOrders,eSignalStatus inStatus);


    int getCurrent_uCode();
    int  calcNext_uCode(bool displayBusses);
    void runPhase(int uCode,bool illustrate); 
    void refreshMicroCodeReg(int uCode);

    void sortieMemoire(int address,bool hilite);
    void entreeMemoire(int address,int data,bool hilite);
    void deselectExtBus();

    ArchiCircuit* mArchiCircuit;
    Sequencer*    mSequencer;
    ImVec2 mPos,mRectPos,mRectSize;
    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ExtBus* mExternalBus;
    RAM*    mRAM;
    Assembleur* mASM;

    MicrocodeFiles* mMicrocodeFiles;
    int mFetchCodeLine;
    bool mAlertMicrocodeTable;

};
