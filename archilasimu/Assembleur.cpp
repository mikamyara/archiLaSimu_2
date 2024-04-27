#include "Assembleur.h"
#include <stdio.h>
#include "stringUtils.h"
#include <iostream>
#include <string.h>
#include <regex>
#include "imGuiApp.h"

OpDesc::OpDesc() {
    RegList = nullptr;

    nbRegs = 0;

    
}

RegOpDesc::RegOpDesc() {
    AdressingModes = nullptr;
    CondModes = nullptr;
    nbAddr = 0;
    nbCond = 0;
    Name[0] = 0;
}






Assembleur::Assembleur() { 
mRAM = nullptr;
menuMnemonics=nullptr;
nbMenuMnemonics=0;

modes["Imm"] = "Immédiat";              modesInv["Immédiat"]        = "Imm";
modes["Dir"] = "Direct";                modesInv["Direct"]          = "Dir";
modes["Ind"] = "Indirect";              modesInv["Indirect"]        = "Ind";
modes["Idx"] = "Indexé";                modesInv["Indexé"]          = "Idx";
modes["Rel"] = "Relatif";               modesInv["Relatif"]         = "Rel";
modes["ImmExt"] = "Immédiat Etendu";    modesInv["Immédiat Etendu"] = "ImmExt";
modes["DirExt"] = "Direct Etendu";      modesInv["Direct Etendu"]   = "DirExt";
modes["IndExt"] = "Indirect Etendu";    modesInv["Indirect Etendu"] = "IndExt";
modes["IdxExt"] = "Indexé Etendu";      modesInv["Indexé Etendu"]   = "IdxExt";
modes["RelExt"] = "Relatif Etendu";     modesInv["Relatif Etendu"]  = "RelExt";
buildMnemonics();
buildMenuStrings();

mShowMnemonicsTable = false;
mShowAssistant = false;


 mColNames = new char*[4];
 for(int k=0;k<4;k++) {
    mColNames[k] = new char [100];
    }
    strcpy(mColNames[0],"OpCode");
    strcpy(mColNames[1],"Mnémonique");
    strcpy(mColNames[2],"Adressage");
    strcpy(mColNames[3],"Commentaire");

    mRows = 501;
    mCols = 4;
 }
 

void 
Assembleur::buildMenuStrings(){

    menuMnemonics = new char*[mOpDescForMenu.size()];
    nbMenuMnemonics = mOpDescForMenu.size();
    int m=0;
 
    //### opcodes
    for(std::map<std::string,OpDesc>::iterator it = mOpDescForMenu.begin(); it != mOpDescForMenu.end(); ++it) {

    std::string opName =  it->first;
    menuMnemonics[m]=new char[100];
    strcpy(menuMnemonics[m],opName.c_str());
    OpDesc& theOpCode = mOpDescForMenu[opName];

    //### regs
    theOpCode.nbRegs = theOpCode.vRegList.size();
    theOpCode.RegList = new char*[theOpCode.vRegList.size()];
    for(int k=0;k<theOpCode.vRegList.size();k++) {
        theOpCode.RegList[k] = new char[100];
        strcpy(theOpCode.RegList[k],theOpCode.vRegList[k].vName.c_str());

        RegOpDesc& theReg = theOpCode.vRegList[k];

        theReg.nbAddr = theReg.vAdressingModes.size();
        theReg.AdressingModes = new char*[theReg.nbAddr];
        
        for(int k=0;k<theReg.vAdressingModes.size();k++) {
            theReg.AdressingModes[k] = new char[100];
            strcpy(theReg.AdressingModes[k],theReg.vAdressingModes[k].c_str());
            }


        theReg.nbCond = theReg.vCondModes.size();
        theReg.CondModes = new char*[theReg.vCondModes.size()];

        for(int k=0;k<theReg.vCondModes.size();k++) {
            theReg.CondModes[k] = new char[100];
            strcpy(theReg.CondModes[k],("("+std::to_string(k)+") "+theReg.vCondModes[k]).c_str());
            }
    //### adressing modes
    
    /*
    mOpDescForMenu[opName].nbAddr = mOpDescForMenu[opName].vAdressingModes.size();
    mOpDescForMenu[opName].AdressingModes = new char*[mOpDescForMenu[opName].vAdressingModes.size()];
    for(int k=0;k<mOpDescForMenu[opName].vAdressingModes.size();k++) {
        mOpDescForMenu[opName].AdressingModes[k] = new char[100];
        strcpy(mOpDescForMenu[opName].AdressingModes[k],mOpDescForMenu[opName].vAdressingModes[k].c_str());
        }

    mOpDescForMenu[opName].nbRegs = mOpDescForMenu[opName].vRegList.size();
    mOpDescForMenu[opName].RegList = new char*[mOpDescForMenu[opName].vRegList.size()];
    for(int k=0;k<mOpDescForMenu[opName].vRegList.size();k++) {
        mOpDescForMenu[opName].RegList[k] = new char[100];
        strcpy(mOpDescForMenu[opName].RegList[k],mOpDescForMenu[opName].vRegList[k].c_str());
        }

    mOpDescForMenu[opName].nbCond = mOpDescForMenu[opName].vCondModes.size();
    mOpDescForMenu[opName].CondModes = new char*[mOpDescForMenu[opName].vCondModes.size()];
    for(int k=0;k<mOpDescForMenu[opName].vCondModes.size();k++) {
        mOpDescForMenu[opName].CondModes[k] = new char[100];
        strcpy(mOpDescForMenu[opName].CondModes[k],("("+std::to_string(k)+") "+mOpDescForMenu[opName].vCondModes[k]).c_str());
        }
        */
        }
    m++;
    }
 
} 

void 
Assembleur::addManyMmnemonics(std::string name, int code,std::vector<std::string> modes,std::string comment){
    for(int k=0;k<modes.size();k++) {
        if(modes[k] != "") {
            addMnemonics(name+","+modes[k],k+code,comment);
        } 
    } 
   
}
 

void 
Assembleur::buildMnemonics() {
    std::vector<std::string> all = {"Imm","Dir","Ind","Idx","Rel"/*,"ImmExt","DirExt","IndExt","IdxExt","RelExt"*/};
    std::vector<std::string> most = {"","Dir","Ind","Idx","Rel",/*"","DirExt","IndExt","IdxExt","RelExt"*/};
    // Math
    addManyMmnemonics("ADD_A",0,all,"Ajoute une valeur, selon mode d'adressage, au registre RA.");
    addManyMmnemonics("ADD_B",5,all,"Ajoute une valeur, selon mode d'adressage, au registre RB.");
    addManyMmnemonics("ADD_X",10,most,"Ajoute une valeur, selon mode d'adressage, à l'adresse du registre RX.");
    addManyMmnemonics("SUB_A",15,all,"Retranche une valeur, selon mode d'adressage, au registre RA.");
    addManyMmnemonics("SUB_B",20,all,"Retranche une valeur, selon mode d'adressage, au registre RB.");
    addManyMmnemonics("SUB_X",25,most,"Retranche une valeur, selon mode d'adressage, à l'adresse du registre RX.");
    addManyMmnemonics("MUL_A",30,all,"Multiplie une valeur, selon mode d'adressage, au registre RA.");
    addManyMmnemonics("MUL_B",35,all,"Multiplie une valeur, selon mode d'adressage, au registre RB");
    addManyMmnemonics("DIV_A",45,all,"Multiplie une valeur, selon mode d'adressage, au registre RA.");
    addManyMmnemonics("DIV_B",50,all,"Multiplie une valeur, selon mode d'adressage, au registre RB.");

    // Bool Bitwise 
    addManyMmnemonics("AND_A",55,all,"Effectue un 'ET' logique bit a bit avec une valeur, selon mode d'adressage, au registre RA.");    
    addManyMmnemonics("AND_B",60,all,"Effectue un 'ET' logique bit a bit avec une valeur, selon mode d'adressage, au registre RB.");    

    addManyMmnemonics("OR_A",70,all,"Effectue un 'OU' logique bit a bit avec une valeur, selon mode d'adressage, au registre RA.");    
    addManyMmnemonics("OR_B",75,all,"Effectue un 'OU' logique bit a bit avec une valeur, selon mode d'adressage, au registre RB.");    

    addManyMmnemonics("XOR_A",85,all,"Effectue un 'OU exlusif' logique bit a bit avec une valeur, selon mode d'adressage, au registre RA.");    
    addManyMmnemonics("XOR_B",90,all,"Effectue un 'OU exlusif' logique bit a bit avec une valeur, selon mode d'adressage, au registre RB.");    


    addManyMmnemonics("LOAD_A",100,all,"Copie une valeur, selon mode d'adressage, dans le registre RA.");
    addManyMmnemonics("LOAD_B",105,all,"Copie une valeur, selon mode d'adressage, dans le registre RB.");
    addManyMmnemonics("LOAD_X",110,most,"Copie une adresse, selon mode d'adressage, dans le registre RX.");
    addManyMmnemonics("LOAD_SP",115,most,"Copie une valeur, selon mode d'adressage, sur la pile (SP).");
 
    addManyMmnemonics("STORE_A",120,most,"Copie la valeur du registre RA dans la RAM, selon mode d'adressage.");
    addManyMmnemonics("STORE_B",125,most,"Copie la valeur du registre RB dans la RAM, selon mode d'adressage.");
    addManyMmnemonics("STORE_X",130,most,"Copie l'adresse contenue dans le registre RX vers la RAM, selon mode d'adressage.");
    addManyMmnemonics("STORE_SP",135,most,"Copie l'adresse contenue dans la pile (SP) vers la RAM, selon mode d'adressage.");

    addManyMmnemonics("PUSH",140,all,"Empile une valeur sur la pile (SP), selon mode d'adressage.");
    addManyMmnemonics("PULL",145,most,"Dépile une valeur sur la pile (SP), selon mode d'adressage.");
    
    addManyMmnemonics("JUMP",150,most,"Saut inconditionnel.");
    addManyMmnemonics("JUMP(A==0)",155,most,"Saut Conditionnel (Saut si RA==0).");
    addManyMmnemonics("JUMP(B==0)",160,most,"Saut Conditionnel (Saut si RB==0).");
    addManyMmnemonics("JUMP(A>0)",165,most,"Saut Conditionnel (Saut si RA>0).");
    addManyMmnemonics("JUMP(B>0)",170,most,"Saut Conditionnel (Saut si RB>0).");
    addManyMmnemonics("JUMP(A:Pair)",175,most,"Saut Conditionnel (Saut si RA est pair).");
    addManyMmnemonics("JUMP(B:Pair)",180,most,"Saut Conditionnel (Saut si RA est impair).");



    addMnemonics("PUSH_A",181,"Empile la valeur contenue dans le registre RA sur la pile (SP).");
    addMnemonics("PUSH_B",182,"Empile la valeur contenue dans le registre RB sur la pile (SP).");
    addMnemonics("PUSH_X",183,"Empile l'adresse contenue dans le registre X sur la pile (SP).");
    addMnemonics("PULL_A",184,"Dépile la valeur au sommet de la pile (SP) vers le registre RA.");
    addMnemonics("PULL_B",185,"Dépile la valeur au sommet de la pile (SP) vers le registre RB.");
    addMnemonics("PULL_X",186,"Dépile la valeur au sommet de la pile (SP) vers le registre X.");
    addMnemonics("CALL",187,"Appel à sous-routine dont l'adresse est passée en paramètre.");
    addMnemonics("RETURN",188,"Sortie de la sous-routine courante.");

    // bool bitwise, reg oriented
    addMnemonics("NOT_A",189,"Inversion logique bit à bit du registre RA.");    
    addMnemonics("NOT_B",190,"Inversion logique bit à bit du registre RB.");    
    addMnemonics("ROR_A",191,"Rotation à droite des bits du registre RA.");    
    addMnemonics("ROR_B",192,"Rotation à droite des bits du registre RB.");    
    addMnemonics("ROL_A",193,"Rotation à gauche des bits du registre RA.");    
    addMnemonics("ROL_B",194,"Rotation à gauche des bits du registre RB.");    
    // math, reg oriented
    addMnemonics("ADDAB",195,"Addition RA + RB, résultat dans RB.");
    addMnemonics("SUBAB",196,"Soustraction RA - RB, résultat dans RB.");    
    addMnemonics("MULAB",197,"Multiplication RA x RB, résultat dans RB.");
    addMnemonics("DIVAB",198,"Division RA / RB, résultat dans RB.");

    addMnemonics("ANDAB",199,"ET bit à bit RA & RB, résultat dans RB.");
    addMnemonics("ORAB",200,"OU bit à bit RA | RB, résultat dans RB.");
    addMnemonics("XORAB",201,"OU exclusif bit à bit RA ^ RB, résultat dans RB.");
    addMnemonics("INC_A",202,"Incrémentation RA.");
    addMnemonics("INC_B",203,"Incrémentation RB.");
    addMnemonics("INC_X",204,"Incrémentation RX.");
    addMnemonics("INC_SP",205,"Incrémentation SP.");
    addMnemonics("DEC_A",206,"Décrémentation RA.");
    addMnemonics("DEC_B",207,"Décrémentation RB.");
    addMnemonics("DEC_X",208,"Décrémentation RX.");
    addMnemonics("DEC_SP",209,"Décrémentation SP.");
    addMnemonics("NOP",210,"Pas d'Opération");

    

}

void
Assembleur::addMnemonics(std::string name, int code,std::string comment){
    mnemonics[name]=code;
    Opcodes[code]=name;


    std::vector<std::string> exploded = splitString(name,',');
    std::string mainName = exploded[0];
    std::string addressMode = "";
    std::string condition = "";
    std::string theRegister = "";

    OpcodesComments[code]=comment;
    
    if (exploded.size()>1) addressMode = exploded[1];

    // check if there are parenthesis
    {
        std::regex regexPattern("([^(]+)\\(([^)]+)\\)");
        std::smatch match;
        if (std::regex_search(mainName, match, regexPattern)) {
            if (match.size() == 3) {
                condition = std::string(match[2].str());
                mainName = std::string(match[1].str());
            }
        }
    }
    // check if there is an underscore for register name
    {
        std::regex regexPattern("([^_]+)_([^_]+)");
        std::smatch match;
        if (std::regex_search(mainName, match, regexPattern)) {
            if (match.size() == 3) {
                theRegister = std::string(match[2].str());
                mainName = std::string(match[1].str());
            }
        }
    }


    //### ops
    if(mOpDescForMenu.find(mainName) == mOpDescForMenu.end()) {
        OpDesc newOp;
     //   newOp.vCondModes.push_back("Aucune");
        mOpDescForMenu[mainName] = newOp;
    }

    OpDesc& theOpCode = mOpDescForMenu[mainName];
    theOpCode.mComment = comment;
    RegOpDesc* theReg = nullptr;
    if(theRegister == "") {theRegister = "Aucun";}
    /*if(theRegister == "") {
        RegOpDesc theTmpReg; theTmpReg.vName = "Aucun";
        theOpCode.vRegList.push_back(theTmpReg);
        theReg = &theOpCode.vRegList[theOpCode.vRegList.size()-1];
    }
    else {*/

    //### registers
    bool notFound = true;
    for(int m=0;m<theOpCode.vRegList.size();m++) {
        if(theOpCode.vRegList[m].vName== theRegister) 
        {notFound = false;
            theReg = &theOpCode.vRegList[m];
        }
    }        
    if(notFound) { RegOpDesc theTmpReg; 
                    theTmpReg.vName =  theRegister;
                    theOpCode.vRegList.push_back(theTmpReg);
                    theReg = &(theOpCode.vRegList[theOpCode.vRegList.size()-1]);

                    }


    //### address Mode
   // if(addressMode != "") {
    if(addressMode == "") addressMode = "Aucun";
        notFound = true;
        for(int m=0;m<theReg->vAdressingModes.size();m++) {
            if(theReg->vAdressingModes[m]== addressMode) notFound = false;
        }
        if(notFound) theReg->vAdressingModes.push_back(addressMode);
    //}


    //### Condition
    if(condition == "") condition = "Aucune";
    //if(condition != "") {
        notFound = true;
        for(int m=0;m<theReg->vCondModes.size();m++) {
            if(theReg->vCondModes[m]== condition) notFound = false;
        }        
        if(notFound) theReg->vCondModes.push_back(condition);
    //}    

   //² }

/*
    if(mOpDescForMenu.find(mainName) == mOpDescForMenu.end()) {
        OpDesc newOp;
        newOp.vCondModes.push_back("Aucune");
        mOpDescForMenu[mainName] = newOp;
    }
 
    if(addressMode != "") {
        bool notFound = true;
        for(int m=0;m<mOpDescForMenu[mainName].vAdressingModes.size();m++) {
            if(mOpDescForMenu[mainName].vAdressingModes[m]== addressMode) notFound = false;
        }
        if(notFound) mOpDescForMenu[mainName].vAdressingModes.push_back(addressMode);
    }
    if(theRegister != "") {
       bool notFound = true;
        for(int m=0;m<mOpDescForMenu[mainName].vRegList.size();m++) {
            if(mOpDescForMenu[mainName].vRegList[m]== theRegister) notFound = false;
        }        
        if(notFound)mOpDescForMenu[mainName].vRegList.push_back(theRegister);
    }
    if(condition != "") {
       bool notFound = true;
        for(int m=0;m<mOpDescForMenu[mainName].vCondModes.size();m++) {
            if(mOpDescForMenu[mainName].vCondModes[m]== condition) notFound = false;
        }        
        if(notFound) mOpDescForMenu[mainName].vCondModes.push_back(condition);
    }

    */
}


void 
Assembleur::buildOpcodeFromMenu(){

 

}


       

void 
Assembleur::drawMnemonicsWindow(ImDrawList* dl, ImVec2 window_pos){

   // ImGui::SetCursorPos (mTablePos);


    if (mShowMnemonicsTable) {

        ImGui::Begin("Opcodes / Mnémoniques",&mShowMnemonicsTable);

        static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable  | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn; // ImGuiTableFlags_Resizable
        static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;

        static int frozen_cols = 1;
        static int frozen_rows = 2;
        static bool firstOpen = true;

        if(firstOpen == true) {
            ImGui::SetWindowSize(toHD(ImVec2(400,500)));
            ImGui::SetWindowPos(toHD(ImVec2(315,265)));             
            firstOpen = false;
        }
        //ImGui::SetCursorPos (pos);
        ImVec2 size = ImGui::GetWindowSize();
        size.x-=16;
        size.y-=50;



        if (ImGui::BeginTable("table_Opcodes", mCols, table_flags, toHD(size)))
        {

            //ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
            for (int n = 0; n < mCols; n++)
                ImGui::TableSetupColumn(mColNames[n], column_flags);
            ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

            ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
            ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
            for (int row = 0; row < mRows; row++)
            {
                if(row>=213)  {  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));}
                else if(row <=180) { if(row%10 <5 ) {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,200,255));}
                                     else {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200,255,255,255));}
                }
                else {
                    if(row%2  ) {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,200,255));}
                                     else {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200,255,255,255));}
                } 
                ImGui::PushID(row);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::Text("%03d", row);
                for (int column = 1; column < mCols; column++)
                    if (ImGui::TableSetColumnIndex(column))
                    {
                        ImGui::PushID(column);
                        char opCodeStr[100]; opCodeStr[0]=0;
                        char adrModeStr[100]; adrModeStr[0]=0;
 

                        if(Opcodes.find(row)!=Opcodes.end()) {
                            std::vector<std::string> parts;
                            parts=splitString(Opcodes[row],',');
                            if(parts.size()>0){
                                strcpy(opCodeStr,parts[0].c_str());}
                            if(parts.size()>1){
                                parts[1] = parts[1]+" ("+modes[parts[1]]+")";
                                strcpy(adrModeStr,parts[1].c_str());}
                       }
                        if(column == 1) {
                            ImGui::PushItemWidth(toHD(70));
                            ImGui::Text("%s",opCodeStr);
                            ImGui::PopItemWidth();
                        } 
                        else if(column == 3) {
                            ImGui::PushItemWidth(toHD(400));
                            ImGui::Text("%s",OpcodesComments[row].c_str());
                            ImGui::PopItemWidth();
                        }
                        else {
                            ImGui::PushItemWidth(toHD(150));
                            ImGui::Text("%s",adrModeStr);
                            ImGui::PopItemWidth();

                        }
                        ImGui::PopID();
                    }
                ImGui::PopID();
                ImGui::PopStyleColor();
            }
            ImGui::EndTable();
        }




        ImGui::End();
    }


}




 

void 
Assembleur::drawOpcodeBuilderWindow(ImDrawList* dl, ImVec2 window_pos){



    if (mShowAssistant) {
        ImGui::Begin("Constructieur d'OpCode (Code-Opération)",&mShowAssistant,ImGuiWindowFlags_NoResize);

        static bool firstOpen = true;

        if(firstOpen == true) {
            ImGui::SetWindowPos(toHD(ImVec2(315,10)));             
            ImGui::SetWindowSize(toHD(ImVec2(600,295))); 
            firstOpen = false;
        }

        
        ImGui::PushItemWidth(toHD(100));
        ImGui::Text("Adresse où écrire l'instruction :");
        static char bufAdr[4]={0};
        ImGui::InputText("##C5",bufAdr,4,ImGuiInputTextFlags_CharsDecimal);
        int theAdress = atoi(bufAdr); 
        ImGui::SameLine();
        bool refreshText = false;
        if(ImGui::Button("+")) {
            theAdress++; refreshText = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("-")) {
            theAdress--;  refreshText = true;
        }
        if(theAdress<0) {
            theAdress = 0;  refreshText = true;
        }
        if(theAdress>100) {
            theAdress = 100;refreshText = true;
        }
        if(refreshText) ::sprintf(bufAdr,"%d",theAdress);




        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Mnémonique   ");
        ImGui::SameLine(); 
        ImGui::Text("Registre           ");
        ImGui::SameLine();               
        ImGui::Text("Adressage        ");
        ImGui::SameLine();  
        ImGui::Text("Condition         ");
        ImGui::SameLine();  
        ImGui::Text("Paramètre");



        // ## MNEMONIQUES
        static int selectMnemonic = 0;
        ImGui::Combo("##C0", &selectMnemonic, menuMnemonics,nbMenuMnemonics);// IM_ARRAYSIZE(items0));

        ImGui::SameLine();  

        // ## REGISTRES


        char** regMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].RegList;
        int nbRegMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].nbRegs;
        static int selectReg = 0;
        if(selectReg >= nbRegMenu) selectReg = 0;
        ImGui::Combo("##C1", &selectReg, regMenu, nbRegMenu);

        ImGui::SameLine();  
    
        // ## MODE ADRESSAGE
        char** adrMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].vRegList[selectReg].AdressingModes;
        int nbAdrMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].vRegList[selectReg].nbAddr;
        static int selectAdr = 0;
        if(selectAdr >= nbAdrMenu) selectAdr = 0;
        ImGui::Combo("##C2", &selectAdr, adrMenu, nbAdrMenu);

        ImGui::SameLine(); 
  
        // ## CONDITION
        char** CondModes = mOpDescForMenu[menuMnemonics[selectMnemonic]].vRegList[selectReg].CondModes;
        int nbCond = mOpDescForMenu[menuMnemonics[selectMnemonic]].vRegList[selectReg].nbCond;
        static int selectedCond = 0;
        //const char* items3[] = { "Choice X", "Choice Y", "Choice Z" };
        if(selectedCond >= nbCond) selectedCond = 0;
        ImGui::Combo("##C3", &selectedCond, CondModes,nbCond);
        
        ImGui::SameLine(); 

        static char buf[5]={0};
        ImGui::InputText("##C4",buf,5,ImGuiInputTextFlags_CharsDecimal);
        ImGui::Spacing();
        ImGui::Spacing();
        int value = atoi(buf);

        ImGui::PopItemWidth();

        ImVec2 thePos = ImGui::GetCursorPos();
    
        std::string comment = "Rôle de l'instruction : "+mOpDescForMenu[menuMnemonics[selectMnemonic]].mComment;
        ImGui::TextWrapped("%s",comment.c_str());
        
        thePos.y+=50;
        ImGui::SetCursorPos(thePos);
        

        // calculer l'instruction
        std::string operation;
        operation+= std::string(menuMnemonics[selectMnemonic]);
        if(nbCond>1) {
            std::regex regexPattern("\\((\\d+)\\) ([^\\)]+)");
            std::smatch match;
            std::string to_test = std::string(CondModes[selectedCond]);
            if (std::regex_search(to_test, match, regexPattern)) {
                if(match[2]!="Aucune")  operation+="("+std::string(match[2])+")";
                }                                                                       
            }
        if(nbRegMenu!=0 && std::string(regMenu[selectReg])!="Aucun") {operation+="_"+std::string(regMenu[selectReg]);}
        if(nbAdrMenu!=0 && std::string(adrMenu[selectAdr])!="Aucun") {operation+=","+std::string(adrMenu[selectAdr]);}

        // Texte en dessous des combobox
        int theInstruction = mnemonics[operation]*10000 + value ;
        std::string fullOp = operation+" "+std::to_string(value);
        ImGui::Text("Instruction assembleur : ");
        ImGui::SameLine();  
        ImGui::Text("%s",fullOp.c_str());
        ImGui::SameLine();  
        ImGui::Text("      Code Instruction : ");
        ImGui::SameLine();  
        ImGui::Text("%d",theInstruction);

        ImGui::Spacing();
        ImGui::Spacing();

        float buttonWidth = 150.0f; // Largeur du bouton
        ImGui::SetCursorPosX(toHD(toLD(ImGui::GetWindowWidth()) - buttonWidth-10)); // Alignement à droite
        if (ImGui::Button("Ecrire l'instruction\n          en RAM", toHD(ImVec2(buttonWidth, 0)))) {
            mRAM->setValue(theAdress,theInstruction);
            mRAM->setRemark(theAdress,fullOp);
            }


    ImGui::End();
    }


}
