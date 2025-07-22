/*
 * ArchiLaSimu - A Web navigator based Von-Neuman Architecture simulator 
 * Copyright (C) 2025 Mikhaël Myara, Arnaud Virazel - Université de Montpellier
 *
 * Ce programme est un logiciel libre : vous pouvez le redistribuer et/ou
 * le modifier selon les termes de la Licence Publique Générale GNU publiée
 * par la Free Software Foundation, soit la version 3 de la licence, soit
 * (à votre choix) toute version ultérieure.
 *
 * Ce programme est distribué dans l’espoir qu’il sera utile,
 * mais SANS AUCUNE GARANTIE, même implicite.
 * Voir la Licence Publique Générale GNU pour plus de détails.
 *
 * Vous devriez avoir reçu une copie de la Licence Publique Générale GNU
 * avec ce programme. Si ce n’est pas le cas, consultez <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <string>
#include <map>
#include "imgui.h"
#include "RAM.h"

class RegOpDesc;

class OpDesc {
    public:
    OpDesc();

    std::vector<RegOpDesc> vRegList;
    std::string mComment;

    char** RegList;
    int nbRegs;

};

class RegOpDesc {
    public:
    RegOpDesc();

    std::string vName;
    std::vector<std::string> vAdressingModes;
    std::vector<std::string> vCondModes;

    char Name[50];
    char** AdressingModes;
    char** CondModes;
    int nbAddr,nbCond;

};
 

class Assembleur {
    public :
        Assembleur();
        void buildMnemonics();
        void addMnemonics(std::string name, int code,std::string comment="");
        void addManyMmnemonics(std::string name, int code,std::vector<std::string> modes,std::string comment);
        virtual void drawMnemonicsWindow(ImDrawList* dl, ImVec2 window_pos);
        virtual void drawOpcodeBuilderWindow(ImDrawList* dl, ImVec2 window_pos);
        void buildMenuStrings();
        void buildOpcodeFromMenu();

       
        std::map<std::string,std::string> modes,modesInv;
        std::map <std::string, int> mnemonics;
        std::map <int, std::string> Opcodes;
        std::map <int, std::string> OpcodesComments;
        

        bool mShowMnemonicsTable;
        char** mColNames;
        int mRows,mCols;

        char** menuMnemonics;
        int nbMenuMnemonics;
        int mSingleAddressingModeBeginAt,mLastOpCode;
        std::map<std::string,OpDesc> mOpDescForMenu;
        
        RAM* mRAM;
               
        bool mShowAssistant;
 };
