#include "ArchiCircuit.h"
#include <iostream>

ArchiCircuit::ArchiCircuit() {

    mPos = ImVec2(300,100);
    RA = new RegisterBus123("RA",IM_COL32(0,50,50,255),ImVec2(500,100)); 
    RB = new RegisterBus123("RB",IM_COL32(0,60,60,255),ImVec2(500,190)); 
    RC = new RegisterBus123("RC",IM_COL32(0,70,70,255),ImVec2(500,280)); 
    RD = new RegisterBus123("RD",IM_COL32(0,80,80,255),ImVec2(500,370)); 

    OP = new RegisterBus123("OP",IM_COL32(20,20,100,255),ImVec2(750,150)); 
    CO = new RegisterBus123("CO",IM_COL32(100,0,0,255),ImVec2(250,160)); 
    RI = new RegisterBus123("RI",IM_COL32(50,0,100,255),ImVec2(150,250)); 
    RX = new RegisterBus123("RX",IM_COL32(75,0,75,255),ImVec2(250,370)); 

    RE = new RegisterBus123("RE",IM_COL32(0,80,0,255),ImVec2(15,15));    RE->mInputs.mPosMode = e_Bottom;
    RAM = new RegisterBus123("RAM",IM_COL32(0,40,0,255),ImVec2(15,334)); 
    std::cout << RAM->mBus1Node << " ";
    RAM->mOutputs.resize(0); RAM->mBus1Node = nullptr; RAM->mBus2Node = nullptr; RAM->mInputs.mPosMode = e_Bottom;
     std::cout << RAM->mBus1Node << " ";


    allRegisters.push_back(RA);
    allRegisters.push_back(RB);
    allRegisters.push_back(RC);
    allRegisters.push_back(RD);

    allRegisters.push_back(OP);
    allRegisters.push_back(CO);
    allRegisters.push_back(RI);
    allRegisters.push_back(RX);

    allRegisters.push_back(RE);
    allRegisters.push_back(RAM);

}   
   
void
ArchiCircuit::Rebuild(){
    int k;
        for(k=0;k<allRegisters.size();k++) {
        allRegisters[k]->Rebuild();
    }

}

void    
ArchiCircuit::draw(ImDrawList* dl, ImVec2 window_pos){
    ImVec2 thePos = ImVec2(window_pos.x + mPos.x,window_pos.y + mPos.y);
    int k;
    for(k=0;k<allRegisters.size();k++) {
        allRegisters[k]->draw(dl,thePos);
    }
}

   /*
   
   RA_GUI = new RegisterWidget(this,&theArchi->RA);RA_GUI->set_position(Vector2i(500, 100)); RA_GUI->Rebuild(); 
     RB_GUI = new RegisterWidget(this,&theArchi->RB);RB_GUI->set_position(Vector2i(500, 190));RB_GUI->Rebuild(); 
     RC_GUI = new RegisterWidget(this,&theArchi->RC);RC_GUI->set_position(Vector2i(500, 280));RC_GUI->Rebuild(); 
     RD_GUI = new RegisterWidget(this,&theArchi->RD);RD_GUI->set_position(Vector2i(500, 370));RD_GUI->Rebuild(); 
    
     OP_GUI = new CombinatorialOperatorWidget(this,&theArchi->OP); OP_GUI->set_position(Vector2i(750, 150));OP_GUI->Rebuild(); 
     CO_GUI = new RegisterWidget(this,&theArchi->CO); CO_GUI->set_position(Vector2i(250, 160));CO_GUI->Rebuild(); 
     RI_GUI = new InstructionRegisterWidget(this,&theArchi->RI);  RI_GUI->set_position(Vector2i(150, 250));RI_GUI->Rebuild();
     RX_GUI = new RegisterWidget(this,&theArchi->RX);RX_GUI->set_position(Vector2i(250,370)); RX_GUI->Rebuild(); 

     RE_GUI = new RegisterWidget(this,&theArchi->RE);               RE_GUI->set_position(Vector2i(15, 15)); RE_GUI->Rebuild(); 
     RAM_GUI = new RegisterWidget(this,&theArchi->RAM); RAM_GUI->set_position(Vector2i(15,334)); RAM_GUI->Rebuild(); 


   RA = RegisterBus123("RA",nvgRGB(0,50,50)); 
    RB = RegisterBus123("RB",nvgRGB(0,60,60));
    RC = RegisterBus123("RC",nvgRGB(0,70,70));
    RD = RegisterBus123("RD",nvgRGB(0,80,80));
    // for management
    OP = RegisterBus123("OP",nvgRGB(20,20,100) );  // OP.mB1B2Disp = eTop;OP.mB3Disp = eBottom;OP.mRectSize={120,110};OP.RebuildArchiViewData() ;
    CO = RegisterBus123("CO",nvgRGB(100,0,0) );
    RI = RegisterBus123("RI",nvgRGB(50,0,100) );   //RI.mOutputs.mPosMode=e_Right; RI.mInputs.mPosMode=e_Left;RI.mRectSize = {190,90};//   RI.mB1B2Disp = eRight;RI.mB3Disp = eLeft;RI.mRectSize = {190,90};RI.RebuildArchiViewData() ;
    RX = RegisterBus123("RX",nvgRGB(75,0,75) );
    //for exchanges
    RE = RegisterBus123("RE",nvgRGB(0,80,0) );   RE.mInputs.mPosMode = e_Bottom;
    RAM = RegisterBus123("RAM",nvgRGB(0,40,0) ); RAM.mOutputs.resize(0); RAM.mBus1Node = nullptr; RAM.mBus2Node = nullptr; RAM.mInputs.mPosMode = e_Bottom;
  */