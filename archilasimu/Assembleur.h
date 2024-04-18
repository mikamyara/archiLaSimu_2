#pragma once
#include <string>
#include <map>
#include "imgui.h"
#include "RAM.h"


class OpDesc {
    public :
    OpDesc();

    // to gather data
    std::vector<std::string> vRegList;
    std::vector<std::string> vAdressingModes;
    std::vector<std::string> vCondModes;

    // for menu
    char** RegList;
    char** AdressingModes;
    char** CondModes;
    int nbRegs,nbAddr,nbCond;

};


class Assembleur {
    public :
        Assembleur();
        void buildMnemonics();
        void addMnemonics(std::string name, int code);
        void addManyMmnemonics(std::string name, int code,std::vector<std::string> modes);
        virtual void drawMnemonicsWindow(ImDrawList* dl, ImVec2 window_pos);
        virtual void drawOpcodeBuilderWindow(ImDrawList* dl, ImVec2 window_pos);
        void buildMenuStrings();
        void buildOpcodeFromMenu();

       
        std::map<std::string,std::string> modes,modesInv;
        std::map < std::string,  int> mnemonics;
        std::map <  int, std::string> Opcodes;

        bool mShowMnemonicsTable;
        char** mColNames;
        int mRows,mCols;

        char** menuMnemonics;
        int nbMenuMnemonics;
        std::map<std::string,OpDesc> mOpDescForMenu;
        
        RAM* mRAM;
        
        
        bool mShowAssistant;
        //ImVec2 mTablePos;
};