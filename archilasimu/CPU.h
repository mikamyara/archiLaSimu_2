#pragma once


#include <imgui.h>
#include "ArchiCircuit.h"
#include "Sequencer.h"
#include "Assembleur.h"

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
    void executeArchiOrders( const std::vector<std::string>& inOrders);
    void hiliteArchiOrders( const std::vector<std::string>& inOrders,eSignalStatus inStatus);


    int getCurrent_uCode();
    int  calcNext_uCode();
    void runPhase(int uCode,bool illustrate); 
    void refreshMicroCodeReg(int uCode);

    void sortieMemoire(int address);
    void entreeMemoire(int address,int data);
    void deselectExtBus();

    ArchiCircuit* mArchiCircuit;
    Sequencer*    mSequencer;
    ImVec2 mPos,mRectPos,mRectSize;
    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ExtBus* mExternalBus;
    RAM*    mRAM;
    Assembleur* mASM;

};