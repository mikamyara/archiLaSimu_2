#include "Sequencer.h"
#include "ArchiTheme.h"
#include <iostream>


Sequencer::Sequencer(CPU* inCPU) {

    mPos = {800,100};
    SeIMS = nullptr;
    Cond = nullptr;
    Fin= nullptr;
    Node* n;
    SeIMS = new MUX("SeIMS",4, {150,35});

    SeIMS->minValue=0;  SeIMS->maxValue=4;
    SeIMS->mInputTextVPos=35;
    Cond = new MUX("RCond",2, {0,43});

    Cond->minValue=0;   Cond->maxValue=1;
    Cond->mInputTextVPos=20;
    Cond->mNameVPos = 0;
    Fin = new MUX("Fin",2, {290,57});

    Fin->minValue=0;   Fin->maxValue=1;
    Fin->mInputTextVPos=20;
    Fin->mNameVPos = 0;
    Plus1 = new IOBox("PLUS1",0, {435,0});

    reskinIOBox(Plus1);
    Plus1->mRectSize= {90,35};
    Plus1->mInputs.mPosMode=e_Right;
    Plus1->mOutputs.mPosMode=e_Left;
    Plus1->mInputs.push_back(n = new Node());
    n->mFather = Plus1;
    Fetch      = new BasicRegister("Fetch",1, {235,105},3);

    reskinIOBox(Fetch);
    Fetch->minValue=0;   Fetch->maxValue=501;
    Fetch->mRectSize= {70,45};
    Fetch->mInputTextVPos=20;
    Fetch->mNameVPos = 0;

    OpCode     = new BasicRegister("RI_OpCode",1, {20,105},3);
    reskinIOBox(OpCode);
    OpCode->minValue=0;   OpCode->maxValue=501;
    OpCode->mRectSize= {100,45};
    OpCode->mInputTextVPos=20;
    OpCode->mNameVPos = 0;
    OpCode->mColor = IM_COL32(75,0,75,255);

    Microcode  = new BasicRegister("Microcode",1, {430,57},3);

    reskinIOBox(Microcode);
    Microcode->minValue=0;   Microcode->maxValue=501;

    Microcode->mInputs.push_back(n=new Node());
    Microcode->mOutputs.mPosMode=e_Right;
    Microcode->mRectSize= {100,45};
    Microcode->mInputTextVPos=20;
    Microcode->mNameVPos = 0;
    n->mFather = Microcode;

    MicrocodeReg = new MicrocodeRegister({35,235});
    allBoxes.push_back(Cond);
    allBoxes.push_back(OpCode);
    allBoxes.push_back(SeIMS);
    allBoxes.push_back(Plus1);
    allBoxes.push_back(Fetch);
    allBoxes.push_back(Fin);
    allBoxes.push_back(Microcode);
    allBoxes.push_back(MicrocodeReg) ;

    mMuxesNames= {"SeIMS","Cond","Fin"};
    mCPU = inCPU;

    mCodeTable = new MicrocodeTable(mCPU);

}


Sequencer::~Sequencer() {

}


void
Sequencer::reskinIOBox(IOBox* outBox) {
    outBox->mColor = gArchiTheme.mMuxColor;
    outBox->mOutputs.mPosMode=e_Top;
    outBox->mWireLen=20;
    Node* n;
    outBox->mOutputs.push_back(n=new Node());
    n->mFather=outBox;
    outBox->mRectSize= {80,70};

}
void
Sequencer::calcBus() {

    ImU32 col = gArchiTheme.mBoxWireColor;
    float w = gArchiTheme.mBusThickness;


    Bus* theBus;
    // Microcode to MicrocodeReg
    Node* theStart = new Node("",Microcode->mOutputs[0]->mLocalPos);
    theBus = new Bus(theStart,"",gArchiTheme.mMicrocodeBusColor,w);
    theBus->mStart->addNewWire(true)->setTarget(new Node("",HVPos(theBus->mStart->mLocalPos,MicrocodeReg->mOutputs[0]->mLocalPos)))
    -> addNewWire(true)->setTarget(MicrocodeReg->mOutputs[0]);
    mBusses.push_back(theBus);
    // Micrococode to Plus1
    theBus = new Bus(Microcode->mOutputs[0],"",col,w) ;
    theBus->mStart->addNewWire(true)->setTarget(new Node("",HVPos(theBus->mStart->mLocalPos,Plus1->mInputs[0]->mLocalPos)))->addNewWire()->setTarget(Plus1->mInputs[0]);
    mBusses.push_back(theBus);
    //  Plus1 to SeIMS to Cond
    theBus = new Bus(Plus1->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(new Node("Plus1SeIMS",HVPos(SeIMS->mInputs[0]->mLocalPos,theBus->mStart->mLocalPos)))->addNewWire(true)->setTarget(SeIMS->mInputs[0]);
    theBus->FindNode("Plus1SeIMS")->addNewWire(true)->setTarget(new Node("",HVPos(Cond->mInputs[0]->mLocalPos,Plus1->mOutputs[0]->mLocalPos)))->addNewWire(true)->setTarget(Cond->mInputs[0]);
    mBusses.push_back(theBus);
    // Cond to SeIMS
    theBus = new Bus(Cond->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(SeIMS->mInputs[1]);
    mBusses.push_back(theBus);
    // SeIMS to Fin
    theBus = new Bus(SeIMS->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Fin->mInputs[0]);
    mBusses.push_back(theBus);
    // Fin to Microcode
    theBus = new Bus(Fin->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Microcode->mInputs[0]);
    mBusses.push_back(theBus);
    // OpCode to SeIMS
    theBus = new Bus(OpCode->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(SeIMS->mInputs[2]);
    mBusses.push_back(theBus);
    // Fetch to Fin
    theBus = new Bus(Fetch->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Fin->mInputs[1]);
    mBusses.push_back(theBus);
    //MicrocodeReg to Cond
    theBus = new Bus(MicrocodeReg->mOutputs[1],"",col,w);
    ImVec2 iVert(0,190);
    theBus->mStart->addNewWire(true)->setTarget(new Node("suiv-",HVPos(theBus->mStart->mLocalPos,iVert)))
    ->addNewWire(true)->setTarget(new Node("",HVPos(Cond->mInputs[1]->mPos,iVert)))
    ->addNewWire(true)->setTarget(Cond->mInputs[1]);
    // Microcodereg to SeIMS
    theBus->FindNode("suiv-")->addNewWire(true)->setTarget(new Node("",HVPos(SeIMS->mInputs[3]->mLocalPos, iVert )))
    ->addNewWire(true)->setTarget(SeIMS->mInputs[3]);
    mBusses.push_back(theBus);

}

void
Sequencer::Rebuild() {
    mCodeTable->Rebuild();

    int k;
    for(k=0; k<allBoxes.size(); k++) {
        allBoxes[k]->Rebuild();
    }

    calcBus();

}

void
Sequencer::draw(ImDrawList* dl, ImVec2 window_pos) {

    ImVec2 thePos = mPos;
    thePos.x += window_pos.x;
    thePos.y += window_pos.y;

    int k;
    for(k=0; k<allBoxes.size(); k++) {
        allBoxes[k]->draw(dl,thePos);
    }
    for(k=0; k<mBusses.size(); k++) {
        mBusses[k]->draw(dl,thePos);
    }

    thePos.x +=430;
    thePos.y+=130;
    char theBuf[] = "Valeurs de Cond : ";
    addAlignedText(dl,thePos,eTextCenter, theBuf,IM_COL32_WHITE,gArchiTheme.mRobotoBoldFont,20) ;
    thePos.y+=18;
    char theBuf1[] = "0:Aucun";
    addAlignedText(dl,thePos,eTextCenter, theBuf1,IM_COL32_WHITE,gArchiTheme.mRobotoFont,20) ;
    thePos.y+=18;
    char theBuf2[] = "1:A==0    2:B==0";
    addAlignedText(dl,thePos,eTextCenter, theBuf2,IM_COL32_WHITE,gArchiTheme.mRobotoFont,20) ;
    thePos.y+=18;
    char theBuf3[] = "3:A>0     4:B >0";
    addAlignedText(dl,thePos,eTextCenter, theBuf3,IM_COL32_WHITE,gArchiTheme.mRobotoFont,20) ;
    thePos.y+=18;
    char theBuf4[] = "5:A pair     6:B pair";
    addAlignedText(dl,thePos,eTextCenter, theBuf4,IM_COL32_WHITE,gArchiTheme.mRobotoFont,20) ;

}

void
Sequencer::drawWidgets(ImDrawList* dl, ImVec2 pos) {
    ImVec2 thePos = mPos;
    thePos.x += pos.x;
    thePos.y += pos.y;
    int k;
    for(k=0; k<allBoxes.size(); k++) {
        allBoxes[k]->drawWidgets(dl,thePos);
    }
    ImVec2 themCodeVec = thePos;
    themCodeVec.y+=312;
    themCodeVec.x+=12;
    mCodeTable->drawWidgets(dl,themCodeVec);
}
