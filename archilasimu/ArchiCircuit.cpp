#include "ArchiCircuit.h"
#include <iostream>
#include "ArchiTheme.h"

ArchiCircuit::ArchiCircuit() {

    mB1signals = {"REB1","RAB1","RBB1","RXB1","SPB1","RIB1","COB1"};
    mB2signals = {"REB2","RAB2","RBB2","RXB2","SPB2","RIB2","COB2"};
    mB3signals = {"eRE" ,"eRA" ,"eRB" ,"eRX" ,"eSP" ,"eRI" ,"eCO","eRAM"};
    mOtherSignals = {"sM","eM"};

    mPos = ImVec2(15,240);
    RA = new RegisterBus123("RA",IM_COL32(0,50,50,255),ImVec2(380,100));
    RB = new RegisterBus123("RB",IM_COL32(0,60,60,255),ImVec2(380,190));
    RX = new RegisterBus123("RX",IM_COL32(0,70,70,255),ImVec2(380,320));
    RX->minValue = 0;
  //  RD = new RegisterBus123("RD",IM_COL32(0,80,80,255),ImVec2(380,370));

    OP = new CombinatorialOperator("OP",IM_COL32(20,20,100,255),ImVec2(630,150));
    OP->minValue = 0;
    CO = new RegisterBus123("CO",IM_COL32(100,0,0,255),ImVec2(130,106));
    CO->minValue = 0;
    RI = new InstructionRegister("RI",IM_COL32(75,0,75,255),ImVec2(130,205));
    RI->minValue = 0;
    SP = new RegisterBus123("SP",IM_COL32(50,0,100,255),ImVec2(130,370));
    SP->minValue = 0;

    RE = new RegisterBus123("RE",IM_COL32(0,80,0,255),ImVec2(15,15));
    RE->mInputs.mPosMode = e_Bottom;

    RAM = new RegisterBus123("RAM",IM_COL32(0,40,0,255),ImVec2(15,334));
    RAM->minValue = 0;

    sensEch = new IOBox("Sens Echange",IM_COL32(0,40,0,255),ImVec2(15,200));     
    sensEch->mInputs.mPosMode = e_Bottom;sensEch->mBorderColor = sensEch->mColor;
    sensEch->mRectSize = ImVec2(90,72);

    RAM->mOutputs.resize(0);
    RAM->mBus1Node = nullptr;
    RAM->mBus2Node = nullptr;
    RAM->mInputs.mPosMode = e_Bottom;

    allRegisters.push_back(RE);
    allRegisters.push_back(RAM);
    allRegisters.push_back(sensEch);
    allRegisters.push_back(CO);
    allRegisters.push_back(RI);
    allRegisters.push_back(SP);
    allRegisters.push_back(RA);
    allRegisters.push_back(RB);
    allRegisters.push_back(RX);
//    allRegisters.push_back(RD);
    allRegisters.push_back(OP);

}


ArchiCircuit::~ArchiCircuit() {} // should be managed


void
ArchiCircuit::Rebuild() {
 
    int k;
    for(k=0; k<allRegisters.size(); k++) {
        allRegisters[k]->Rebuild();
    }
 
    calcBus();
 
/*
    mBus1->changePortionStatus("RIB1",selected);
    mBus2->changePortionStatus("RBB2",selected);
    mBus3->changePortionStatus("eRE",selected);
*/
}



void
ArchiCircuit::calcBus() {
    // delete existing ones : should also delete nodes, may require some management like a flag "canBeDeleted"
    int k;
    for(k=0; k<mBuses.size(); k++) {
        delete mBuses[k];
    }
    mBuses.resize(0);
    mBus1 = nullptr;
    mBus2 = nullptr;
    mBus3 = nullptr;


    mBus1 = new Bus(OP->mBus1Node,"Bus1",gArchiTheme.mBus1Color,gArchiTheme.mBusThickness);
    Node* theNode = mBus1->mStart;
    theNode->addNewWire()->setTarget(new Node("OPB1REB1",HVPos(OP->mBus1Node->mLocalPos,RE->mBus1Node->mLocalPos)))
    ->addNewWire()->setTarget(new Node("RAB1REB1",HVPos(RA->mBus1Node->mLocalPos,RE->mBus1Node->mLocalPos)))
    ->addNewWire()->setTarget(new Node("COB1REB1",HVPos(CO->mBus1Node->mLocalPos,RE->mBus1Node->mLocalPos)))
    ->addNewWire()->setTarget(RE->mBus1Node);

    // data registers branch
    mBus1->FindNode("RAB1REB1")->addNewWire()->setTarget(RA->mBus1Node)
    ->addNewWire()->setTarget(RB->mBus1Node)
    ->addNewWire()->setTarget(RX->mBus1Node);
    //->addNewWire()->setTarget(RD->mBus1Node);
    // others registers branch
    mBus1->FindNode("COB1REB1")->addNewWire()->setTarget(CO->mBus1Node)
    ->addNewWire()->setTarget(RI->mBus1Node)
    ->addNewWire()->setTarget(SP->mBus1Node);


    mBus2 = new Bus(OP->mBus2Node,"Bus2",gArchiTheme.mBus2Color,gArchiTheme.mBusThickness);
    theNode = mBus2->mStart;
    theNode->addNewWire()->setTarget(new Node("OPB2REB2",HVPos(OP->mBus2Node->mLocalPos,RE->mBus2Node->mLocalPos)))
    ->addNewWire()->setTarget(new Node("RAB2REB2",HVPos(HExt(RA->mBus2Node->mLocalPos,30),RE->mBus2Node->mLocalPos)))
    ->addNewWire()->setTarget(new Node("COB2REB2",HVPos(HExt(CO->mBus2Node->mLocalPos,30),RE->mBus2Node->mLocalPos)))
    ->addNewWire()->setTarget(RE->mBus2Node);
    // data registers branch
    mBus2->FindNode("RAB2REB2")->addNewWire()->setTarget(new Node("RAB2REB2-b",HExt(RA->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(RA->mBus2Node);
    mBus2->FindNode("RAB2REB2-b")->addNewWire()->setTarget(new Node("RAB2RBB2",HExt(RB->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(RB->mBus2Node);
    mBus2->FindNode("RAB2RBB2")->addNewWire()->setTarget(new Node("RBB2RCB2",HExt(RX->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(RX->mBus2Node);
    //mBus2->FindNode("RBB2RCB2")->addNewWire()->setTarget(new Node("RCB2RDB2",HExt(RD->mBus2Node->mLocalPos,30)))
    //->addNewWire()->setTarget(RD->mBus2Node);
    // others registers branch
    mBus2->FindNode("COB2REB2")->addNewWire()->setTarget(new Node("COB2REB2-b",HExt(CO->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(CO->mBus2Node);
    mBus2->FindNode("COB2REB2-b")->addNewWire()->setTarget(new Node("COB2RIB2",HExt(RI->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(RI->mBus2Node);
    mBus2->FindNode("COB2RIB2")->addNewWire()->setTarget(new Node("RIB2RXB2",HExt(SP->mBus2Node->mLocalPos,30)))
    ->addNewWire()->setTarget(SP->mBus2Node);



    ImVec2 savePos;
    mBus3 = new Bus(OP->mBus3Node,"Bus3",gArchiTheme.mBus3Color,gArchiTheme.mBusThickness);
    theNode = mBus3->mStart;
    theNode->addNewWire()->setTarget(new Node("OPB3XB3",savePos = VExt(OP->mBus3Node->mLocalPos,70)))
    ->addNewWire()->setTarget(new Node("OPB3RXB3-b",savePos = HVPos(RX->mBus3Node->mLocalPos,savePos)))
    //->addNewWire()->setTarget(RD->mBus3Node)
    ->addNewWire()->setTarget(RX->mBus3Node)
    ->addNewWire()->setTarget(RB->mBus3Node)
    ->addNewWire()->setTarget(RA->mBus3Node);


    mBus3->FindNode("OPB3RXB3-b")->addNewWire()->setTarget(new Node("OPB3RIB3",savePos=HVPos(RI->mBus3Node->mLocalPos,savePos)))
    ->addNewWire()->setTarget(new Node("OPB3RXB3",HVPos(savePos,SP->mBus3Node->mLocalPos)))
    ->addNewWire()->setTarget(RI->mBus3Node)
    ->addNewWire()->setTarget(new Node("RIB3COB3",savePos=HVPos(RI->mBus3Node->mLocalPos,CO->mBus3Node->mLocalPos)))
    ->addNewWire()->setTarget(new Node("COB3REB3",HVPos(savePos,RE->mBus3Node->mLocalPos)))
    ->addNewWire()->setTarget(RE->mBus3Node);



    mBus3->FindNode("OPB3RXB3")->addNewWire()->setTarget(SP->mBus3Node);
    mBus3->FindNode("OPB3RIB3")->addNewWire()->setTarget(RAM->mBus3Node);
    mBus3->FindNode("RIB3COB3")->addNewWire()->setTarget(CO->mBus3Node);

    mBuses.push_back(mBus1);
    mBuses.push_back(mBus2);
    mBuses.push_back(mBus3);


}



void
ArchiCircuit::draw(ImDrawList* dl, ImVec2 window_pos) {
    ImVec2 thePos = ImVec2(window_pos.x + mPos.x,window_pos.y + mPos.y);
    int k;
    for(k=0; k<allRegisters.size(); k++) {
        allRegisters[k]->draw(dl,thePos);
    }
    for(k=0; k<mBuses.size(); k++) {
        mBuses[k]->draw(dl,thePos);
    }
}



void
ArchiCircuit::drawWidgets(ImDrawList* dl, ImVec2 pos) {
    ImVec2 thePos = ImVec2(pos.x + mPos.x,pos.y + mPos.y);
    int k;
    for(k=0; k<allRegisters.size(); k++) {
        allRegisters[k]->drawWidgets(dl,thePos);
    }
}


