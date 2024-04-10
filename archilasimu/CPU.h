#pragma once


#include <imgui.h>
#include "ArchiCircuit.h"
#include "Sequencer.h"
class CPU {
public :
    CPU();
    void    draw(ImDrawList* dl, ImVec2 window_pos);
    void drawWidgets(ImDrawList* dl,ImVec2 pos);
    void getAdressingModes(std::vector<std::string>& shortName,std::vector<std::string>& longName);
    void Rebuild();

    ArchiCircuit* mArchiCircuit;
    Sequencer*    mSequencer;
    ImVec2 mPos,mRectPos,mRectSize;
    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;

};