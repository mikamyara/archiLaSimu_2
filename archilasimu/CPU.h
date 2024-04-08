#pragma once


#include <imgui.h>
#include "ArchiCircuit.h"
class CPU {
public :
    CPU();
    void    draw(ImDrawList* dl, ImVec2 window_pos);
    ArchiCircuit* mArchiCircuit;
    ImVec2 mPos,mRectPos,mRectSize;
    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ImU32  mPinBackground, mPinBorder;
};