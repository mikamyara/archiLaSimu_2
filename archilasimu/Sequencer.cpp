#include "Sequencer.h"
#include "ArchiTheme.h"
#include <iostream>


Sequencer::Sequencer(CPU* inCPU){
 
    mPos = {800,100};
    SeIMS = nullptr; Cond = nullptr; Fin= nullptr;
    SeIMS = new MUX("SeIMS",4,{140,30});    allBoxes.push_back(SeIMS); SeIMS->minValue=0;  SeIMS->maxValue=4;
    Fin = new MUX("Fin",2,{0,40});          allBoxes.push_back(Fin);   Fin->minValue=0;    Fin->maxValue=1;
    Cond = new MUX("Cond",2,{290,60});      allBoxes.push_back(Cond);  Cond->minValue=0;   Cond->maxValue=1;
    Plus1 = new IOBox("PLUS1",0,{435,0});   allBoxes.push_back(Plus1); reskinIOBox(Plus1); Plus1->mRectSize={90,35}; Plus1->mInputs.mPosMode=e_Right;Plus1->mOutputs.mPosMode=e_Left;
    Plus1->mInputs.push_back(new Node());     
    Fetch      = new BasicRegister("Fetch",1,{230,120},3); allBoxes.push_back(Fetch); reskinIOBox(Fetch);
    OpCode     = new BasicRegister("OpCode",1,{30,120},3); allBoxes.push_back(OpCode); reskinIOBox(OpCode);
    Microcode  = new BasicRegister("Microcode",1,{430,55},3); allBoxes.push_back(Microcode); reskinIOBox(Microcode);  
    Microcode->mInputs.push_back(new Node());      Microcode->mOutputs.mPosMode=e_Right; Microcode->mRectSize={100,70}; 

    mMuxesNames={"SeIMS","Fin","Cond"};
    mCPU = inCPU;

    mCodeTable = new MicrocodeTable(mCPU);

}


Sequencer::~Sequencer(){

}


void
Sequencer::reskinIOBox(IOBox* outBox) {
    outBox->mColor = gArchiTheme.mMuxColor;   outBox->mOutputs.mPosMode=e_Top;outBox->mWireLen=20; outBox->mOutputs.push_back(new Node()); outBox->mRectSize={80,70}; 

}
void
Sequencer::calcBus() {

    ImU32 col = gArchiTheme.mBoxWireColor;
    float w = gArchiTheme.mBusThickness;      

    // Micrococode to Plus1
    Bus* theBus = new Bus(Microcode->mOutputs[0],"",col,w) ;
    theBus->mStart->addNewWire(true)->setTarget(new Node("",HVPos(theBus->mStart->mLocalPos,Plus1->mInputs[0]->mLocalPos)))->addNewWire(true)->setTarget(Plus1->mInputs[0]);
    mBusses.push_back(theBus);
    //  Plus1 to SeIMS to Fin
    theBus = new Bus(Plus1->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(new Node("Plus1SeIMS",HVPos(SeIMS->mInputs[0]->mLocalPos,theBus->mStart->mLocalPos)))->addNewWire(true)->setTarget(SeIMS->mInputs[0]);
    theBus->FindNode("Plus1SeIMS")->addNewWire(true)->setTarget(new Node("",HVPos(Fin->mInputs[0]->mLocalPos,Plus1->mOutputs[0]->mLocalPos)))->addNewWire(true)->setTarget(Fin->mInputs[0]);
    mBusses.push_back(theBus);
    // Fin to SeIMS
    theBus = new Bus(Fin->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(SeIMS->mInputs[1]);
    mBusses.push_back(theBus);
    // SeIMS to Cond
    theBus = new Bus(SeIMS->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Cond->mInputs[0]);
    mBusses.push_back(theBus);
    // Cond to Microcode
    theBus = new Bus(Cond->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Microcode->mInputs[0]);
    mBusses.push_back(theBus);
    // OpCode to SeIMS
    theBus = new Bus(OpCode->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(SeIMS->mInputs[2]);
    mBusses.push_back(theBus);
    // Fetch to Cond
    theBus = new Bus(Fetch->mOutputs[0],"",col,w);
    theBus->mStart->addNewWire(true)->setTarget(Cond->mInputs[1]);
    mBusses.push_back(theBus);

}

void
Sequencer::Rebuild(){
    mCodeTable->Rebuild();

    int k;
    for(k=0;k<allBoxes.size();k++) {
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
    for(k=0;k<allBoxes.size();k++) {
        allBoxes[k]->draw(dl,thePos);
        }
    for(k=0;k<mBusses.size();k++) {
       mBusses[k]->draw(dl,thePos);        
    }
}

void     
Sequencer::drawWidgets(ImDrawList* dl, ImVec2 pos){
    ImVec2 thePos = mPos;
    thePos.x += pos.x;
    thePos.y += pos.y;
    int k;
    for(k=0;k<allBoxes.size();k++) {
        allBoxes[k]->drawWidgets(dl,thePos);
    }
    ImVec2 themCodeVec = thePos;
    themCodeVec.y+=310;
    themCodeVec.x+=10;
    mCodeTable->drawWidgets(dl,themCodeVec);
}
