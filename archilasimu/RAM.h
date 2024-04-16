#pragma once
#include <imgui.h>
#include "Assembleur.h"

class RAM {
public:
    RAM();
    void    draw(ImDrawList* dl, ImVec2 window_pos);
    void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);
    int     getValue(int address);
    void    setValue(int address,int data);
    


    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ImVec2 mPos;
    ImVec2 mRectPos,mRectSize;
    char** mColNames;
    int mCols,mRows;
    char** mData;
    char** mHelpers;

    Assembleur* mASM;
};