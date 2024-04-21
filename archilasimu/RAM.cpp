#include "RAM.h"
#include <iostream>
#include "ArchiTheme.h"
#include <string.h>
#include "Assembleur.h"

RAM::RAM()  {
    mASM = nullptr;

    int k;
    mCols=3;
    mRows=128;

    mData = new char*[mRows];
    mHelpers = new char*[mRows];

    for(k=0; k<mRows; k++) {
        mData[k] = new char[10];
        mData[k][0] = 0;
        mHelpers[k] = new char[100];
        mHelpers[k][0] = 0;
    }

    mColNames = new char*[mCols];
    for(k=0; k<mCols; k++) {
        mColNames[k] = new char[30];
    }
    strcpy(mColNames[0],"Adresses");
    strcpy(mColNames[1],"Données");
    strcpy(mColNames[2],"Remarques");



    mPos = ImVec2(10,10);
    mRectPos = ImVec2(0,0);
    mRectSize = ImVec2(300,730);

    mGlobalBackground   = IM_COL32(80, 80, 80,255);
    mSubPanelBackground = IM_COL32(60, 60, 60,255);
    mBorderColor = IM_COL32(140, 140, 140,255);
}

void
RAM::draw(ImDrawList* dl, ImVec2 window_pos) {

    ImVec2 P1,P2;
    ImVec2 thePos = ImVec2(mPos.x + window_pos.x, mPos.y + window_pos.y);
    P1.x = thePos.x + mRectPos.x;
    P1.y = thePos.y + mRectPos.y;
    P2.x = P1.x + mRectSize.x;
    P2.y = P1.y + mRectSize.y;

    dl->AddRectFilled(toHD(P1),toHD(P2), mGlobalBackground);
    dl->AddRect(toHD(P1),toHD(P2), mBorderColor, 0.0f, ImDrawFlags_None, toHD(4.0f));

    ImVec2 theCPUNamePos = ImVec2( (P1.x + P2.x)/2-30,thePos.y+10 );
    addAlignedText(dl,toHD(theCPUNamePos),eTextCenter,"RAM",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,toHD(48));

    P1.x += 10;
    P1.y+=130;
    P2.x = P1.x + 280;
    P2.y = P1.y + 590;

    dl->AddRectFilled(toHD(P1),toHD(P2), mSubPanelBackground);


}


void
RAM::drawWidgets(ImDrawList* dl, ImVec2 window_pos) {
    ImVec2 pos = ImVec2(mPos.x + window_pos.x+10, mPos.y + window_pos.y+70);


    ImGui::SetCursorPos (toHD(pos));
    ImGui::Checkbox("Assistant : Insertion d'un Opcode\nà partir de sa Mnémonique", &mASM->mShowAssistant);
    mASM->drawOpcodeBuilderWindow(dl,pos);


    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable  | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn; // ImGuiTableFlags_Resizable
    static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;

    static int frozen_cols = 1;
    static int frozen_rows = 2;


    pos.y+=60;
    ImGui::SetCursorPos (toHD(pos));
    if (ImGui::BeginTable("table_RAM", mCols, table_flags, toHD(ImVec2(280, 590))))
    {
        //ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = 0; n < mCols; n++)
            ImGui::TableSetupColumn(mColNames[n], column_flags);
        ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

        ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
        for (int row = 0; row < mRows; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%03d", row);
            for (int column = 1; column < mCols; column++)
                if (ImGui::TableSetColumnIndex(column))
                {
                    ImGui::PushID(column);

                    if(column == 1) {
                        ImGui::PushItemWidth(toHD(70));
                        ImGui::InputText("",mData[row], 8, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::PopItemWidth();
                    } else {
                        ImGui::PushItemWidth(toHD(140));
                        ImGui::InputText("",mHelpers[row], 100);
                        ImGui::PopItemWidth();

                    }
                    ImGui::PopID();
                }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }


}



int     
RAM::getValue(int address) {
    if (address>=0 && address <mRows) {
    int value=-1;
    value = atoi(mData[address]);

    return value;
    }
    return -1;
}

void    
RAM::setValue(int address,int data){
    if (address>=0 && address <mRows) {
        ::sprintf(mData[address],"%d",data);
    }

}


void    
RAM::setRemark(int address, std::string remark)
{
        if (address>=0 && address <mRows) {
            ::sprintf(mHelpers[address],"%s",remark.c_str());
        }
}
