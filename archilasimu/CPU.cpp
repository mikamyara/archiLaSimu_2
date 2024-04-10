#include "CPU.h"
#include "ArchiTheme.h"

CPU::CPU() {
    //pinSize = ImVec2(8,4);
    mPos = ImVec2(450,10);
    mRectPos = ImVec2(0,0);
    //mRectSize = ImVec2(1000,500);
    mArchiCircuit = new ArchiCircuit();

    mSequencer = new Sequencer(this);

    mGlobalBackground   = IM_COL32(80, 80, 80,255);
    mSubPanelBackground = IM_COL32(60, 60, 60,255);
    mBorderColor = IM_COL32(140, 140, 140,255);

}
void
CPU::Rebuild() {
    mArchiCircuit->Rebuild();
    mSequencer->Rebuild();
}

void    
CPU::draw(ImDrawList* dl, ImVec2 window_pos) {



    ImVec2 P1global,P2global,P1arch,P2arch,P1seq,P2seq;
    ImVec2 thePos = ImVec2(mPos.x + window_pos.x , mPos.y + window_pos.y); 
    P1global.x = thePos.x + mRectPos.x;
    P1global.y = thePos.y + mRectPos.y;
    P1arch = P1global; P1arch.x+=10;P1arch.y+=70;
    P2arch = P1arch ;  P2arch.x +=765;  P2arch.y+=650;

    P1seq.x += P2arch.x + 10;  P1seq.y = P1arch.y;
    P2seq = P1seq;   P2seq.x += 600;  P2seq.y = P2arch.y;

    P2global = P2seq; P2global.x+=10;P2global.y+=10;


    dl->AddRectFilled (P1global, P2global, mGlobalBackground);  
    dl->AddRect(P1global, P2global, mBorderColor, 0.0f, ImDrawFlags_None, 4.0f);

    dl->AddRectFilled (P1arch, P2arch, mSubPanelBackground);  
    dl->AddRect(P1arch, P2arch, mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    dl->AddRectFilled (P1seq, P2seq, mSubPanelBackground);  
    dl->AddRect(P1seq, P2seq, mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    ImVec2 theCPUNamePos = ImVec2( (P1global.x + P2global.x)/2,thePos.y+10 );
    addAlignedText(dl,theCPUNamePos,eTextCenter,"CPU",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,48);

    ImVec2 theArchiNamePos = ImVec2((P1arch.x + P2arch.x)/2,P1arch.y+3);
    addAlignedText(dl,theArchiNamePos,eTextCenter,"Architecture",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,35);
   
    ImVec2 theSequenserNamePos = ImVec2((P1seq.x + P2seq.x)/2-40,theArchiNamePos.y);
    addAlignedText(dl,theSequenserNamePos,eTextCenter,"Séquenceur",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,35);
   

    mArchiCircuit->draw(dl,thePos);
    mSequencer->draw(dl,thePos);
}


void     
CPU::drawWidgets(ImDrawList* dl, ImVec2 pos){
    pos.x +=mPos.x;
    pos.y +=mPos.y;

    mArchiCircuit->drawWidgets(dl,pos);
    mSequencer->drawWidgets(dl,pos);
}

void
CPU::getAdressingModes(std::vector<std::string>& shortNames,std::vector<std::string>& longNames){
    shortNames = {"Imm","Dir","Ind","Idx","Rel"};
    longNames = {"Immédiat","Direct","Indirect","Indexé","Relatif"};

}
