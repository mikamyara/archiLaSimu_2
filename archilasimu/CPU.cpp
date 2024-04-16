#include "CPU.h"
#include "ArchiTheme.h"
#include <stdlib.h>
#include <iostream>
#include "stringUtils.h"
#include "RAM.h"

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
    mExternalBus = nullptr;
    mRAM = nullptr;
    mASM = nullptr;

}
void
CPU::Rebuild() {
    mArchiCircuit->Rebuild();
    mSequencer->Rebuild();
}

void
CPU::draw(ImDrawList* dl, ImVec2 window_pos) {

    ImVec2 P1global,P2global,P1arch,P2arch,P1seq,P2seq,P1act,P2act;
    ImVec2 thePos = ImVec2(mPos.x + window_pos.x, mPos.y + window_pos.y);
    P1global.x = thePos.x + mRectPos.x;
    P1global.y = thePos.y + mRectPos.y;
    P1act = P1global;
    P1act.x+=10;
    P1act.y+=70;
    P2act = P1act;
    P2act.x+=765;
    P2act.y += 140;

    P1arch = P1global;
    P1arch.x+=10;
    P1arch.y+=220;
    P2arch = P1arch ;
    P2arch.x +=765;
    P2arch.y+=500;

    P1seq.x = P2arch.x   + 10;
    P1seq.y = P1global.y +70;
    P2seq = P1seq;
    P2seq.x += 600;
    P2seq.y = P2arch.y;

    P2global = P2seq;
    P2global.x+=10;
    P2global.y+=10;
    


    dl->AddRectFilled (P1global, P2global, mGlobalBackground);
    dl->AddRect(P1global, P2global, mBorderColor, 0.0f, ImDrawFlags_None, 4.0f);

    dl->AddRectFilled (P1act, P2act, mSubPanelBackground);
    dl->AddRect(P1act, P2act, mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    dl->AddRectFilled (P1arch, P2arch, mSubPanelBackground);
    dl->AddRect(P1arch, P2arch, mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    dl->AddRectFilled (P1seq, P2seq, mSubPanelBackground);
    dl->AddRect(P1seq, P2seq, mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    ImVec2 theCPUNamePos = ImVec2( (P1global.x + P2global.x)/2,thePos.y+10 );
    addAlignedText(dl,theCPUNamePos,eTextCenter,"CPU",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,48);

    ImVec2 theArchiNamePos = ImVec2((P1arch.x + P2arch.x)/2,P1arch.y+3);
    addAlignedText(dl,theArchiNamePos,eTextCenter,"Architecture",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,35);

    ImVec2 theSequenserNamePos = ImVec2((P1seq.x + P2seq.x)/2-40,P1seq.y+3);
    addAlignedText(dl,theSequenserNamePos,eTextCenter,"Séquenceur",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,35);

    mASM->drawMnemonicsWindow(dl,thePos);
    mArchiCircuit->draw(dl,thePos);
    mSequencer->draw(dl,thePos);
}


void
CPU::drawWidgets(ImDrawList* dl, ImVec2 pos) {
    ImVec2 savePos = pos;
    savePos.x += mPos.x;  savePos.y += mPos.y;


    pos = savePos;
    pos.x  +=17 ;    pos.y +=77;

    ImGui::SetCursorPos (pos);

    ImGui::PushStyleVar (ImGuiStyleVar_ChildBorderSize, 0);	// Ajustez la taille de la bordure ici
    ImGui::BeginChild ("Commandes CPU", ImVec2 (460, 125), true);
    ImGui::PopStyleVar ();

    if (ImGui::BeginTabBar ("ASM ou Phase")) {

            if (ImGui::BeginTabItem ("Microcode Phase par Phase"))
            {
                if (ImGui::Button("Exécuter la prochaine\n phase du microcode"))
                {
                int code = getCurrent_uCode();
                //std::cout << "NEW CODE : "<< code << "\n";
                runPhase(code,selected);

                }    
            ImGui::EndTabItem ();

            }

            if (ImGui::BeginTabItem ("Assembleur Pas à Pas"))
            {
                // Contenu de l'onglet 2
                if (ImGui::Button(" Exécuter la prochaine\ninstruction assembleur"))
                {
               // std::cout << "BB\n";
                }    
            ImGui::EndTabItem ();

            }
        

        ImGui::EndTabBar ();
    }

    ImGui::EndChild ();

    pos = savePos;
    pos.y +=93;
    pos.x +=485;

    ImGui::SetCursorPos (pos);
    //ImGui::PushItemWidth(mInputTextWidth);
    if (ImGui::Button("            Reset\n              CPU              "))
    {
        Reset();
    }
    pos.y +=55;
    ImGui::SetCursorPos (pos);

    ImGui::Checkbox("Table Opcodes\net Mnémoniques", &mASM->mShowMnemonicsTable);

    pos = savePos;
    pos.y +=93;
    pos.x +=500+140;

    ImGui::SetCursorPos (pos);    
    if (ImGui::Button("   Charger Table   \n    Microcode"))
    {
    //std::cout << "BB\n";
    }   
    pos.y +=50;
    ImGui::SetCursorPos (pos);         
    if (ImGui::Button("Enregistrer Table\n     Microcode"))
    {
    //std::cout << "CC\n";
    }    

    pos = savePos;
    mArchiCircuit->drawWidgets(dl,pos);
    mSequencer->drawWidgets(dl,pos);

}

void
CPU::getAdressingModes(std::vector<std::string>& shortNames,std::vector<std::string>& longNames) {
    shortNames = {"Imm","Dir","Ind","Idx","Rel"};
    longNames = {"Immédiat","Direct","Indirect","Indexé","Relatif"};

}

void 
CPU::setRegisterValue(std::string regName,int inValue) {
int k;
for(k=0;k<mArchiCircuit->allRegisters.size();k++) {
    if(regName == mArchiCircuit->allRegisters[k]->mName) {
        BasicRegister* reg = dynamic_cast<BasicRegister*>(mArchiCircuit->allRegisters[k]);
 
        reg->setValue(inValue);
        }
    }
for(k=0;k<mSequencer->allBoxes.size();k++) {
    if(regName == mSequencer->allBoxes[k]->mName) {
        BasicRegister* reg = dynamic_cast<BasicRegister*>(mSequencer->allBoxes[k]);
 
        reg->setValue(inValue);
        }
    }

}


int  
CPU::getRegisterValue(std::string regName) {
int k;
for(k=0;k<mArchiCircuit->allRegisters.size();k++) {
        if(toUpper(regName) == toUpper(mArchiCircuit->allRegisters[k]->mName)) {
            BasicRegister* reg = dynamic_cast<BasicRegister*>(mArchiCircuit->allRegisters[k]); 
            return reg->getValue();           
        }
    }
for(k=0;k<mSequencer->allBoxes.size();k++) {
        if(toUpper(regName) == toUpper(mSequencer->allBoxes[k]->mName)) {
            BasicRegister* reg = dynamic_cast<BasicRegister*>(mSequencer->allBoxes[k]); 
            return reg->getValue();           
        }
    }

return -1;
}



void 
CPU::Reset() {
    setRegisterValue("RA",10);
    setRegisterValue("RB",15);
    setRegisterValue("RC",0);
    setRegisterValue("RD",0);
    setRegisterValue("RE",0);
    setRegisterValue("RI",0);
    setRegisterValue("RX",0);
    setRegisterValue("CO",0);
    setRegisterValue("RAM",0);
    setRegisterValue("Cond",0);
    setRegisterValue("Fin",0);
    setRegisterValue("SeIMS",0);
    setRegisterValue("OpCode",0);
    setRegisterValue("Fetch",498);
    setRegisterValue("Microcode",0);

    // ui related
    mArchiCircuit->mBus1->changeBusStatus(normal);
    mArchiCircuit->mBus2->changeBusStatus(normal);
    mArchiCircuit->mBus3->changeBusStatus(normal);


    mSequencer->mCodeTable->insertByExpression("000:005:2:5:1: RAB1 RBB2 ADD eCO eRC");

    refreshMicroCodeReg(getCurrent_uCode());

}

int CPU::getCurrent_uCode() {
    return  getRegisterValue("Microcode");
}

int  
CPU::calcNext_uCode(){

    //regs
    int microcode = getRegisterValue("Microcode");
    int Fetch = getRegisterValue("Fetch");
    int OpCode = getRegisterValue("OpCode");
    int Suiv = getRegisterValue("Suiv");

    //Muxes
    int Fin = getRegisterValue("Fin");
    //std::cout << "FIN" << Fin << "\n";
    int SeIMS =getRegisterValue("SeIMS");
    int RCond = getRegisterValue("RCond");

  //  std::cout <<"ucode " << microcode << " Fetch " << Fetch << " OpCode " << OpCode <<" Suiv " <<Suiv << " Fin " <<Fin<< " SeIMS " << SeIMS << " RCond " << RCond << "\n";

    // algo
    int plus1 = microcode + 1;
    
    if(Fin==1) return Fetch;
    else { // Fin=0, so look at SeIMS 
     if(SeIMS==0) return plus1;
     if(SeIMS==2) return OpCode;
     if(SeIMS==3) return Suiv;
     if(SeIMS==1) { // SeIMS == 1 and Fin == 0, so look at cond
        if(RCond==0) return plus1;
        else return Suiv;
       }
    }

    return -1;
}

void 
CPU::refreshMicroCodeReg(int uCode){
    MicrocodeTable* T = mSequencer->mCodeTable;
    mSequencer->MicrocodeReg->uCode = uCode;
    mSequencer->MicrocodeReg->SeIMS = T->mSeIMS[uCode];
    mSequencer->MicrocodeReg->Cond = T->mCond[uCode];
    mSequencer->MicrocodeReg->Fin = T->mSignals[uCode][T->sigToCol["FIN"]]?1:0;
    int addrSuiv;
    sscanf(T->mAdrSuiv[uCode],"%d",&addrSuiv);
    mSequencer->MicrocodeReg->suiv = addrSuiv;

    mSequencer->MicrocodeReg->ordres = mSequencer->mCodeTable->signalsToString(uCode);
    if(trim(mSequencer->MicrocodeReg->ordres) == "") {mSequencer->MicrocodeReg->ordres='-';}

}

void 
CPU::runPhase(int uCode,bool illustrate){

    setRegisterValue("SeIMS",mSequencer->MicrocodeReg->SeIMS);
    setRegisterValue("Cond",mSequencer->MicrocodeReg->Cond);
    setRegisterValue("Fin",mSequencer->MicrocodeReg->Fin);


/*
    MicrocodeTable* T = mSequencer->mCodeTable;
    setRegisterValue("SeIMS",T->mSeIMS[uCode]);
    setRegisterValue("Cond",T->mCond[uCode]);
    int addrSuiv;
    sscanf(T->mAdrSuiv[uCode],"%d",&addrSuiv);
    //setRegisterValue("Suiv",addrSuiv);
    mSequencer->MicrocodeReg->suiv = addrSuiv;
    setRegisterValue("Fin",T->mSignals[uCode][T->sigToCol["FIN"]]?1:0);
*/

    std::string str= mSequencer->mCodeTable->signalsToString(uCode);
    std::vector < std::string > ordersList;
    explode(str, ordersList);

    executeArchiOrders(ordersList);

    if(illustrate){
        hiliteArchiOrders( ordersList,selected);
    }

    int newCode = calcNext_uCode();
    setRegisterValue("Microcode",newCode);
    refreshMicroCodeReg(newCode);
   /* mSequencer->MicrocodeReg->uCode = newCode;
    mSequencer->MicrocodeReg->SeIMS = getRegisterValue("SeIMS");
    mSequencer->MicrocodeReg->Fin = getRegisterValue("Fin");
    mSequencer->MicrocodeReg->Cond = getRegisterValue("Cond");
*/

 /* 

class MicrocodeRegister: public IOBox {
public :
    MicrocodeRegister();
    MicrocodeRegister(ImVec2 inPos);
    virtual void drawName(ImDrawList* dl,ImVec2 window_pos);
    void  RebuildNodesCoords(IOBoxNodes& ioNodesList    );
    virtual void     drawSingleValue(ImDrawList* dl,ImVec2 pos,int hoffset,std::string formatStr,std::string name,int value);
    virtual void     drawOutputNodes(ImDrawList* dl,ImVec2 window_pos);

    int uCode, suiv,SeIMS,Cond,Fin;
    std::string ordres;
};*/





}


void
CPU::executeArchiOrders(const std::vector<std::string>& inOrders) {
    {
        std::vector<std::string>& signals = mArchiCircuit->mOtherSignals;
        deselectExtBus();        
        // first run sM or eM
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    int address = getRegisterValue("RAM");
                    if(signals[l]=="sM") {
                        sortieMemoire(address);
                    }
                    if(signals[l]=="eM") {
                    int data = getRegisterValue("RE");
                        entreeMemoire(address,data);
                    }

                }
            }
        }
    }
    int X=-1,Y=-1,Z=-1;
    // bus 1
    {
        std::vector<std::string>& signals = mArchiCircuit->mB1signals;
        Bus* bus = mArchiCircuit->mBus1;
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    std::string regName =  bus->FindNode(signals[l])->mFather->mName;
                    X = getRegisterValue(regName);
                }
            }
        }
    }
    // bus 2
    {
        std::vector<std::string>& signals = mArchiCircuit->mB2signals;
        Bus* bus = mArchiCircuit->mBus2;
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    std::string regName =  bus->FindNode(signals[l])->mFather->mName;
                    Y = getRegisterValue(regName);
                }
            }
        }    
    }
    // op
    {
        std::map<std::string, CombinatorialOperator::FunctionPtr>& ops = mArchiCircuit->OP->mOperations;
        CombinatorialOperator::FunctionPtr opFunc = nullptr;
        CombinatorialOperator* opReg = mArchiCircuit->OP;
        for(int k=0;k<inOrders.size();k++ ) {
            if (ops.find(inOrders[k]) == ops.end()) {
            // not found
            } else {
                opFunc = ops[inOrders[k]];
                Z = opFunc(X,Y);
                sprintf(opReg->buf,"%s",inOrders[k].c_str());
            }
        }
    }        
    // bus 3
    {
        std::vector<std::string>& signals  = mArchiCircuit->mB3signals;
        Bus* bus = mArchiCircuit->mBus3;
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    std::string regName =  bus->FindNode(signals[l])->mFather->mName;
                    setRegisterValue(regName,Z);
                }
            }
        }   
    }         
}

void
CPU::hiliteArchiOrders(const std::vector<std::string>& inOrders,eSignalStatus inStatus) {

    {
        Bus* bus = mArchiCircuit->mBus1;
        std::vector<std::string>& signals = mArchiCircuit->mB1signals;
        bus->changeBusStatus(normal);
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    bus->changePortionStatus(signals[l],inStatus);
                }
            }
        }
    }
    {
        Bus* bus =  bus = mArchiCircuit->mBus2;
        std::vector<std::string>& signals = mArchiCircuit->mB2signals;
        bus->changeBusStatus(normal);
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                     bus->changePortionStatus(signals[l],inStatus);
                }
            }
        }
    }
    {
         Bus* bus = mArchiCircuit->mBus3;
        std::vector<std::string>& signals  = mArchiCircuit->mB3signals;
        bus->changeBusStatus(normal);
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                     bus->changePortionStatus(signals[l],inStatus);
                 }
            }
        }
    }
 
}


void 
CPU::deselectExtBus() {

    // ui related
    mExternalBus->mAdressBusSelected = false;
    mExternalBus->mRAMToCPU = false;
    mExternalBus->mCPUToRAM = false;

}

void 
CPU::sortieMemoire(int address){


    // ui related
    mExternalBus->mAdressBusSelected = true;
    mExternalBus->mRAMToCPU = true;
    int data = mRAM->getValue(getRegisterValue("RAM"));
    setRegisterValue("RE",data);


}

void 
CPU::entreeMemoire(int address,int data){
    mExternalBus->mAdressBusSelected = true;
    mExternalBus->mCPUToRAM = true;
    mRAM->setValue(getRegisterValue("RAM"),getRegisterValue("RE"));
}
