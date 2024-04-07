#include "ArchiCircuit.h"
#include <iostream>

ArchiCircuit::ArchiCircuit() {

    mPos = ImVec2(300,100);
    RA = new RegisterBus123("RA",IM_COL32(0,50,50,255),ImVec2(500,100)); 
    RB = new RegisterBus123("RB",IM_COL32(0,60,60,255),ImVec2(500,190)); 
    RC = new RegisterBus123("RC",IM_COL32(0,70,70,255),ImVec2(500,280)); 
    RD = new RegisterBus123("RD",IM_COL32(0,80,80,255),ImVec2(500,370)); 

    OP = new CombinatorialOperator("OP",IM_COL32(20,20,100,255),ImVec2(750,150)); 
    CO = new RegisterBus123("CO",IM_COL32(100,0,0,255),ImVec2(250,160)); 
    RI = new InstructionRegister("RI",IM_COL32(50,0,100,255),ImVec2(150,250)); 
    RX = new RegisterBus123("RX",IM_COL32(75,0,75,255),ImVec2(250,370)); 

    RE = new RegisterBus123("RE",IM_COL32(0,80,0,255),ImVec2(15,15));    RE->mInputs.mPosMode = e_Bottom;
    RAM = new RegisterBus123("RAM",IM_COL32(0,40,0,255),ImVec2(15,334)); 
    //std::cout << "YOUHOU" << RAM->mBus1Node << " ";
    RAM->mOutputs.resize(0); RAM->mBus1Node = nullptr; RAM->mBus2Node = nullptr; RAM->mInputs.mPosMode = e_Bottom;
     //std::cout << RAM->mBus1Node << "\n";


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

