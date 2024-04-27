#include "CPU.h"
#include "ArchiTheme.h"
#include <stdlib.h>
#include <iostream>
#include "stringUtils.h"
#include "RAM.h"
#include "imGuiApp.h"
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
    mMicrocodeFiles = nullptr;
    mAlertMicrocodeTable = false;

}
void
CPU::Rebuild() {
    mArchiCircuit->Rebuild();
    mSequencer->Rebuild();
    mMicrocodeFiles = new MicrocodeFiles();
    mMicrocodeFiles->mTable = mSequencer->mCodeTable;    
    

    mFetchCodeLine = mSequencer->mCodeTable->mRows-3;

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
    


    dl->AddRectFilled (toHD(P1global), toHD(P2global), mGlobalBackground);
    dl->AddRect(toHD(P1global), toHD(P2global), mBorderColor, 0.0f, ImDrawFlags_None, 4.0f);

    dl->AddRectFilled (toHD(P1act), toHD(P2act), mSubPanelBackground);
    dl->AddRect(toHD(P1act), toHD(P2act), toHD(mBorderColor), 0.0f, ImDrawFlags_None, 0.5f);

    dl->AddRectFilled (toHD(P1arch), toHD(P2arch), mSubPanelBackground);
    dl->AddRect(toHD(P1arch), toHD(P2arch), mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    dl->AddRectFilled (toHD(P1seq), toHD(P2seq), mSubPanelBackground);
    dl->AddRect(toHD(P1seq), toHD(P2seq), mBorderColor, 0.0f, ImDrawFlags_None, 0.5f);

    ImVec2 theCPUNamePos = ImVec2( (P1global.x + P2global.x)/2,thePos.y+10 );
    addAlignedText(dl,toHD(theCPUNamePos),eTextCenter,"CPU",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,toHD(48));

    ImVec2 theArchiNamePos = ImVec2((P1arch.x + P2arch.x)/2,P1arch.y+3);
    addAlignedText(dl,toHD(theArchiNamePos),eTextCenter,"Architecture",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,toHD(35));

    ImVec2 theSequenserNamePos = ImVec2((P1seq.x + P2seq.x)/2-40,P1seq.y+3);
    addAlignedText(dl,toHD(theSequenserNamePos),eTextCenter,"Séquenceur",IM_COL32(255,255,255,255),gArchiTheme.mRobotoBoldFont,toHD(35));

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

    ImGui::SetCursorPos (toHD(pos));

    ImGui::PushStyleVar (ImGuiStyleVar_ChildBorderSize, 0);	// Ajustez la taille de la bordure ici
    ImGui::BeginChild ("Commandes CPU", toHD(ImVec2 (460, 125)), true);
    ImGui::PopStyleVar ();

    if (ImGui::BeginTabBar ("ASM ou Phase")) {

            if (ImGui::BeginTabItem ("Microcode Phase par Phase"))
            {   ImVec2 theLocalPos =toLD(ImGui::GetCursorPos());
                theLocalPos.x+=50;
                theLocalPos.y+=10;
                ImGui::SetCursorPos(toHD(theLocalPos));
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60,150,60,255)); 
                if (ImGui::Button("Exécuter une nouvelle\n phase du microcode"))
                {
                int code = getCurrent_uCode();
                runPhase(code,true);

                }    
            ImGui::PopStyleColor();
            ImGui::EndTabItem ();

            }

            if (ImGui::BeginTabItem ("Assembleur Pas à Pas"))
            { ImVec2 theLocalPos = toLD(ImGui::GetCursorPos());
                theLocalPos.x+=50;
                theLocalPos.y+=10;
                ImGui::SetCursorPos(toHD(theLocalPos));
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60,150,60,255)); 

                // Contenu de l'onglet 2
                if (ImGui::Button(" Exécuter une nouvelle\ninstruction assembleur")) 
                { 
                bool again = true;
                int cpt =0;
                do {
                    int code;
                    code = getCurrent_uCode();
                    runPhase(code,false);
                    cpt++;
                    if(mSequencer->MicrocodeReg->Fin == 1) again = false;
                    if(cpt>10000) again = false;
                    } while(again);
                if( mSequencer->MicrocodeReg->Fin!=1) {
                    mAlertMicrocodeTable = true;
                }

                }    
            ImGui::PopStyleColor();
            ImGui::EndTabItem ();

            }
        

        ImGui::EndTabBar ();
    }

    ImGui::EndChild ();

    pos = savePos;
    pos.y +=93;
    pos.x +=485;

    ImGui::SetCursorPos (toHD(pos));
    //ImGui::PushItemWidth(mInputTextWidth);
    if (ImGui::Button("            Reset\n              CPU              "))
    {
        Reset();
    }
    pos.y +=55;
    ImGui::SetCursorPos (toHD(pos));

    ImGui::Checkbox("Table Opcodes\net Mnémoniques", &mASM->mShowMnemonicsTable);

    pos = savePos;
    pos.y +=93;
    pos.x +=500+140;

    ImGui::SetCursorPos (toHD(pos));    
    if (ImGui::Button("   Charger Table   \n      Microcode"))
    {mMicrocodeFiles->Upload();
    }   
    pos.y +=50;
    ImGui::SetCursorPos (toHD(pos));         
    if (ImGui::Button("Enregistrer Table\n       Microcode"))
    {mMicrocodeFiles->Download();
    }    

    pos = savePos;
    mArchiCircuit->drawWidgets(dl,pos);
    mSequencer->drawWidgets(dl,pos);

    if(mAlertMicrocodeTable) {
        ImGui::SetNextWindowPos(ImVec2(300, 100));  
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);        
        ImGui::Begin("Erreur dans la table des microcodes !",&mAlertMicrocodeTable,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
        ImGui::PushItemWidth(300);          

        const char* texte = "L'instruction assembleur en cours n'a pas abouti "
                            "après 10 000 itérations dans la table du microcode. "
                            "Elle est sûrement mal implémentée.\n"
                            "Il est aussi possible que vous ayez lancé l'exécution d'une " 
                            "instruction assembleur non implémentée dans la table de microcode, "
                            "ou avec un OpCode non défini.";

        ImGui::TextWrapped("%s", texte);
        if(ImGui::Button("D'accord.")) { mAlertMicrocodeTable = false;}
        ImGui::PopItemWidth();  // Restaure la largeur précédente
        ImGui::End(); 
    }
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
    setRegisterValue("RA",0);
    setRegisterValue("RB",0);
    setRegisterValue("RX",0);
    //setRegisterValue("RD",0);
    setRegisterValue("RE",0);
    setRegisterValue("RI",0);
    setRegisterValue("SP",0);
    setRegisterValue("CO",0);
    setRegisterValue("RAM",0);
    ::sprintf(mArchiCircuit->OP->buf,"%s","");
    setRegisterValue("Cond",0);
    setRegisterValue("Fin",0);
    setRegisterValue("SeIMS",0);
    setRegisterValue("RI_OpCode",0);
    setRegisterValue("Fetch",mFetchCodeLine);
    setRegisterValue("Microcode",mFetchCodeLine);

    // ui related
    mArchiCircuit->mBus1->changeBusStatus(normal);
    mArchiCircuit->mBus2->changeBusStatus(normal);
    mArchiCircuit->mBus3->changeBusStatus(normal);

    // add fetch
    mSequencer->mCodeTable->insertByExpression (std::to_string(mFetchCodeLine+0)+":000:0:0:0: COB1 XS eRAM");
    mSequencer->mCodeTable->insertByExpression (std::to_string(mFetchCodeLine+1)+":000:0:0:0: sM");
    mSequencer->mCodeTable->insertByExpression (std::to_string(mFetchCodeLine+2)+":000:2:0:0: REB1 XS eRI");



/*
    mSequencer->mCodeTable->insertByExpression("100:321:3:0:0: RIB1 Xs eRA");
    mSequencer->mCodeTable->insertByExpression("110:321:3:0:0: RIB1 Xs eRB");
    mSequencer->mCodeTable->insertByExpression("321:0:0:0:1: COB1 PLUS1 eCO");
    mSequencer->mCodeTable->insertByExpression("322:0:0:0:1: COB1 PLUS1 eCO");
*/
   // mRAM->setValue(0,1002560);
   // mRAM->setValue(1,1100128);
    

    refreshMicroCodeReg(0);
    mSequencer->displayBusses("CleanOnly");
    mExternalBus->mAdressBusSelected = false;
    mExternalBus->mRAMToCPU = false;
    mExternalBus->mCPUToRAM = false;

}

int CPU::getCurrent_uCode() {
    return  getRegisterValue("Microcode");
}

int  
CPU::calcNext_uCode(bool displayBusses){

    //regs
    int microcode = getRegisterValue("Microcode");
    int Fetch = getRegisterValue("Fetch");
    int OpCode = getRegisterValue("RI_OpCode");
    int Suiv = mSequencer->MicrocodeReg->suiv;

    //Muxes
    int Fin = mSequencer->MicrocodeReg->Fin; 
    int SeIMS =mSequencer->MicrocodeReg->SeIMS; 
    int RCond = getRegisterValue("RCond");

    // algo
    int plus1 = microcode + 1;
    
    if(Fin==1) { 
        if(displayBusses)mSequencer->displayBusses("Fetch");
        return Fetch;
    }
    else { // Fin=0, so look at SeIMS 
     if(SeIMS==0) {
        if(displayBusses)mSequencer->displayBusses("Plus1Seims");
        return plus1;
     }
     if(SeIMS==2) {
        if(displayBusses)mSequencer->displayBusses("OpCode");
        return OpCode;
     }
     if(SeIMS==3) {
        if(displayBusses)mSequencer->displayBusses("Suiv");
        return Suiv;
     }
     if(SeIMS==1) { // SeIMS == 1 and Fin == 0, so look at cond
        if(RCond==0)  {
            if(displayBusses)mSequencer->displayBusses("Plus1Rcond");        
            return plus1;
        }
        else {
            if(displayBusses)mSequencer->displayBusses("SuivRcond");        
            return Suiv;
        }
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

    refreshMicroCodeReg(uCode);

    setRegisterValue("SeIMS",mSequencer->MicrocodeReg->SeIMS);
    setRegisterValue("Fin",mSequencer->MicrocodeReg->Fin);
    // eval conditions
    int A,B,RCond = 0;

    A = getRegisterValue("RA");     B = getRegisterValue("RB");
    switch(mSequencer->MicrocodeReg->Cond) {
        case 0 :
            RCond = 0;
            break;
        case 1 : RCond = ((A==0)?1:0);
            break;
        case 2 : RCond = ((A==0)?1:0);
            break;
        case 3 : RCond = ((A>0)?1:0);
            break;
        case 4 : RCond = ((B>0)?1:0);
            break;
        case 5 : RCond = ((A%2==0)?1:0);
            break;
        case 6 : RCond = ((B%2==0)?1:0);
            break;
    }

    setRegisterValue("RCond",RCond);

    std::string str= mSequencer->mCodeTable->signalsToString(uCode);
    std::vector < std::string > ordersList;
    explode(str, ordersList);

    executeArchiOrders(ordersList,illustrate);


    if(illustrate){
        hiliteArchiOrders( ordersList,selected);
    }

    setRegisterValue("RI_OpCode",mArchiCircuit->RI->getOpCodeValue());

    int newCode = calcNext_uCode(illustrate);
    setRegisterValue("Microcode",newCode);
    
  
}


void
CPU::executeArchiOrders(const std::vector<std::string>& inOrders,bool hilite) {
    {
        std::vector<std::string>& signals = mArchiCircuit->mOtherSignals;
        deselectExtBus();        
        // first run sM or eM
        for(int k=0;k<inOrders.size();k++ ) {
            for(int l=0;l<signals.size();l++) {
                if(signals[l] == inOrders[k]) {
                    int address = getRegisterValue("RAM");
                    if(signals[l]=="sM") {
                        sortieMemoire(address,hilite);
                    }
                    if(signals[l]=="eM") {
                    int data = getRegisterValue("RE");
                        entreeMemoire(address,data,hilite);
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
    {   bool opExecuted = false;
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
                opExecuted = true;
            }
        }
        if (opExecuted == false) {
            sprintf(opReg->buf,"%s"," ");
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
CPU::sortieMemoire(int address,bool hilite){


    // ui related
    if(hilite){
        mExternalBus->mAdressBusSelected = true;
        mExternalBus->mRAMToCPU = true;
    }
    int data = mRAM->getValue(getRegisterValue("RAM"));
    setRegisterValue("RE",data);


}

void 
CPU::entreeMemoire(int address,int data,bool hilite){
    if(hilite){
        mExternalBus->mAdressBusSelected = true;
        mExternalBus->mCPUToRAM = true;
    }
    mRAM->setValue(getRegisterValue("RAM"),getRegisterValue("RE"));
}
