#include "Assembleur.h"
#include <stdio.h>
#include "stringUtils.h"
#include <iostream>
#include <string.h>
#include <regex>

OpDesc::OpDesc() {
    RegList = nullptr;
    AdressingModes = nullptr;
    CondModes = nullptr;
    nbRegs = 0;
    nbAddr = 0;
    nbCond = 0;
    
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


 mColNames = new char*[3];
 for(int k=0;k<3;k++) {
    mColNames[k] = new char [100];
    }
    strcpy(mColNames[0],"OpCode");
    strcpy(mColNames[1],"Mnémonique");
    strcpy(mColNames[2],"Adressage");

    mRows = 501;
    mCols = 3;
 }
 

void 
Assembleur::buildMenuStrings(){
    menuMnemonics = new char*[mOpDescForMenu.size()];
    nbMenuMnemonics = mOpDescForMenu.size();
    int m=0;
 
    for(std::map<std::string,OpDesc>::iterator it = mOpDescForMenu.begin(); it != mOpDescForMenu.end(); ++it) {

    std::string opName =  it->first;
    menuMnemonics[m]=new char[100];
    strcpy(menuMnemonics[m],opName.c_str());

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
        
    m++;
    }

} 

void 
Assembleur::addManyMmnemonics(std::string name, int code,std::vector<std::string> modes){
    for(int k=0;k<modes.size();k++) {
        if(modes[k] != "") {
            addMnemonics(name+","+modes[k],k+code);
        } 
    } 
}
 

void 
Assembleur::buildMnemonics() {
    std::vector<std::string> all = {"Imm","Dir","Ind","Idx","Rel","ImmExt","DirExt","IndExt","IdxExt","RelExt"};
    std::vector<std::string> most = {"","Dir","Ind","Idx","Rel","","DirExt","IndExt","IdxExt","RelExt"};
    addManyMmnemonics("M+A=>A",0,all);
    addManyMmnemonics("M+B=>B",10,all);
    addMnemonics("A+B=>B",20);
    addMnemonics("A-B=>B",30);
    addMnemonics("INC_A",40);
    addMnemonics("INC_B",50);
    addMnemonics("INC_C",60);
    addMnemonics("INC_D",70);
    addMnemonics("INC_X",80);
    addManyMmnemonics("CALL",90,most);
    addManyMmnemonics("LOAD_A",100,all);
    addManyMmnemonics("LOAD_B",110,all);
    addManyMmnemonics("LOAD_C",120,all);
    addManyMmnemonics("LOAD_D",130,all);
    addManyMmnemonics("LOAD_X",140,all);
    addMnemonics("NOP",150);
    addManyMmnemonics("RETURN",160,most);
    addManyMmnemonics("STORE_A",170,most);
    addManyMmnemonics("STORE_B",180,most);
    addManyMmnemonics("STORE_C",190,most);
    addManyMmnemonics("STORE_D",200,most);
    addManyMmnemonics("STORE_X",210,most);
    addManyMmnemonics("JUMP",220,most);
    addManyMmnemonics("JUMP(A==0)",230,most);
    addManyMmnemonics("JUMP(B==0)",240,most);
    addManyMmnemonics("JUMP(A>0)",250,most);
    addManyMmnemonics("JUMP(B>0)",260,most);
    addManyMmnemonics("JUMP(A:Pair)",270,most);
    addManyMmnemonics("JUMP(B:Pair)",280,most);
    addMnemonics("AxB=>B",290);
    addMnemonics("AetB=>B",300);
    addMnemonics("AouB=>B",310);
    addMnemonics("AxorB=>B",320);
}

void
Assembleur::addMnemonics(std::string name, int code){
    mnemonics[name]=code;
    Opcodes[code]=name;


    std::vector<std::string> exploded = splitString(name,',');
    std::string mainName = exploded[0];
    std::string addressMode = "";
    std::string condition = "";
    std::string theRegister = "";
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
}


void 
Assembleur::buildOpcodeFromMenu(){

 

}



       
       

void 
Assembleur::drawMnemonicsWindow(ImDrawList* dl, ImVec2 window_pos){

   // ImGui::SetCursorPos (mTablePos);


    if (mShowMnemonicsTable) {
        ImGui::Begin("Opcodes / Mnémoniques",&mShowMnemonicsTable);


        static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable  | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn; // ImGuiTableFlags_Resizable
        static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;

        static int frozen_cols = 1;
        static int frozen_rows = 2;
        static bool firstOpen = true;

        if(firstOpen == true) {
            ImGui::SetWindowSize(ImVec2(300,500));
            ImGui::SetWindowPos(ImVec2(315,265));             
            firstOpen = false;
        }
        //ImGui::SetCursorPos (pos);
        ImVec2 size = ImGui::GetWindowSize();
        size.x-=16;
        size.y-=50;
        if (ImGui::BeginTable("table_Opcodes", mCols, table_flags, size))
        {

            //ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
            for (int n = 0; n < mCols; n++)
                ImGui::TableSetupColumn(mColNames[n], column_flags);
            ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

            ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
            ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
            for (int row = 0; row < mRows; row++)
            {
                if(row>=321)  {  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));}
                else if(row%20 <10) {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,200,255));}
                else {ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200,255,255,255));}
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
                            ImGui::PushItemWidth(70);
                            ImGui::Text("%s",opCodeStr);
                            ImGui::PopItemWidth();
                        } else {
                            ImGui::PushItemWidth(140);
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
            ImGui::SetWindowPos(ImVec2(315,10));             
            ImGui::SetWindowSize(ImVec2(600,245)); 
            firstOpen = false;
        }


        ImGui::PushItemWidth(100);
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
            char** adrMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].AdressingModes;
            int nbAdrMenu = mOpDescForMenu[menuMnemonics[selectMnemonic]].nbAddr;
            static int selectAdr = 0;
            if(selectAdr >= nbAdrMenu) selectAdr = 0;
            ImGui::Combo("##C2", &selectAdr, adrMenu, nbAdrMenu);

            ImGui::SameLine(); 

            // ## CONDITION
            char** CondModes = mOpDescForMenu[menuMnemonics[selectMnemonic]].CondModes;
            int nbCond = mOpDescForMenu[menuMnemonics[selectMnemonic]].nbCond;
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
            if(nbRegMenu!=0) {operation+="_"+std::string(regMenu[selectReg]);}
            if(nbAdrMenu!=0) {operation+=","+std::string(adrMenu[selectAdr]);}

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
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth-10); // Alignement à droite
            if (ImGui::Button("Ecrire l'instruction\n          en RAM", ImVec2(buttonWidth, 0))) {
                mRAM->setValue(theAdress,theInstruction);
                mRAM->setRemark(theAdress,fullOp);
              }


        ImGui::End();
        }


}
