#pragma once
#include <imgui.h>

class RAM {
public:
    RAM();
    void    draw(ImDrawList* dl, ImVec2 window_pos);
    void    drawWidgets(ImDrawList* dl, ImVec2 window_pos);

    ImU32  mGlobalBackground,mSubPanelBackground,mBorderColor;
    ImVec2 mPos;
    ImVec2 mRectPos,mRectSize;
    char** mColNames;
    int mCols,mRows;
    char** mData;
    char** mHelpers;
};