#include "RAM.h"
#include <iostream>
#include "ArchiTheme.h"

RAM::RAM()  {
mPos = ImVec2(10,10);
mRectPos = ImVec2(0,0);
mRectSize = ImVec2(350,600);

mGlobalBackground   = IM_COL32(80, 80, 80,255);
mSubPanelBackground = IM_COL32(60, 60, 60,255);
mBorderColor = IM_COL32(140, 140, 140,255);}

void   
RAM::draw(ImDrawList* dl, ImVec2 window_pos) {
    ImVec2 P1,P2;
    ImVec2 thePos = ImVec2(mPos.x + window_pos.x, mPos.y + window_pos.y); 
    P1.x = thePos.x + mRectPos.x;
    P1.y = thePos.y + mRectPos.y;
    P2.x = P1.x + mRectSize.x;
    P2.y = P1.y + mRectSize.y;

    std::cout <<P1.x << " " << P2.y << " "<<P2.x << " " << P2.y << "\n";

    dl->AddRectFilled(P1,P2, mGlobalBackground);  
    dl->AddRect(P1,P2, mBorderColor, 0.0f, ImDrawFlags_None, 4.0f);

    ImVec2 theCPUNamePos = ImVec2( (P1.x + P2.x)/2-30,thePos.y+10 );
    addAlignedText(dl,theCPUNamePos,eTextCenter,"RAM",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,48);


}
